// SPDX-License-Identifier: GPL-2.0
/*
 * otgen.c - Roco OTG GPIO regulator/switch driver for Casper VIA S48 / F716
 *
 * Reconstructed from Casper stock kernel binary (stock-Image) disassembly
 * at addresses 0x89be00 (probe), 0x89be64 (otg_en_set), and 0x131dc34 (init).
 *
 * Hardware binding:
 *   compatible = "roco,otgen";
 *   gpio_otg_en = <&pio 80 0>;
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>

static int gpio_otg_en = -1;

void otg_en_set(int enable)
{
	if (gpio_is_valid(gpio_otg_en)) {
		gpio_direction_output(gpio_otg_en, enable ? 1 : 0);
	}
}
EXPORT_SYMBOL(otg_en_set);

static const struct of_device_id otgen_of_match[] = {
	{ .compatible = "roco,otgen", },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, otgen_of_match);

static int otgen_probe(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	int gpio;
	int ret;

	if (!np || !of_match_node(otgen_of_match, np))
		return -ENODEV;

	gpio = of_get_named_gpio(np, "gpio_otg_en", 0);
	if (!gpio_is_valid(gpio)) {
		dev_err(&pdev->dev, "Invalid gpio_otg_en: %d\n", gpio);
		return -EINVAL;
	}

	ret = devm_gpio_request(&pdev->dev, gpio, "GPIO_OTG_EN");
	if (ret) {
		dev_err(&pdev->dev, "Failed to request GPIO_OTG_EN: %d\n", ret);
		return ret;
	}

	gpio_otg_en = gpio;
	gpio_direction_output(gpio_otg_en, 0);

	dev_info(&pdev->dev, "otgen probe success, gpio_otg_en=%d\n", gpio_otg_en);
	return 0;
}

static int otgen_remove(struct platform_device *pdev)
{
	if (gpio_is_valid(gpio_otg_en)) {
		gpio_direction_output(gpio_otg_en, 0);
		gpio_otg_en = -1;
	}
	pr_err("OTG: Unregister otg driver done\n");
	return 0;
}

static struct platform_driver otgen_driver = {
	.probe = otgen_probe,
	.remove = otgen_remove,
	.driver = {
		.name = "otgen",
		.of_match_table = otgen_of_match,
		.owner = THIS_MODULE,
	},
};

static int __init otgen_init(void)
{
	pr_notice("OTG: Register OTG driver\n");
	if (platform_driver_register(&otgen_driver)) {
		pr_err("OTG: failed to register disp driver\n");
		return -ENODEV;
	}
	return 0;
}

static void __exit otgen_exit(void)
{
	platform_driver_unregister(&otgen_driver);
}

subsys_initcall(otgen_init);
module_exit(otgen_exit);

MODULE_DESCRIPTION("Roco OTG Power Enable Driver");
MODULE_LICENSE("GPL v2");
