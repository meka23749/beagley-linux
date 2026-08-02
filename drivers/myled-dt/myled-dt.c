// myled-dt.c - A LED platform driver bound via the device tree
// beagley-linux — Embedded Linux From Scratch
//
// Cross-compile against the 7.1.5 kernel (see Makefile), then:
//   sudo insmod myled-dt.ko    # only probes if the DT has a "steve,myled" node
//
// Device tree node to add (in k3-am67a-beagley-ai.dts, inside the root node):
//   steve_led: steve-led {
//       compatible = "steve,myled";
//       label = "hello-from-devicetree";
//       status = "okay";
//   };

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/leds.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Steve");
MODULE_DESCRIPTION("A LED platform driver bound via device tree");
MODULE_VERSION("0.1");

static int led_state = 0;
static struct led_classdev myled_cdev;

// Called by the leds-class framework on write (e.g. echo 1 > .../brightness)
static void myled_set(struct led_classdev *cdev, enum led_brightness b)
{
    led_state = b;
    printk(KERN_INFO "myled-dt: brightness set to %d\n", b);
    // A full driver would toggle a GPIO here (gpiod_set_value).
}

static enum led_brightness myled_get(struct led_classdev *cdev)
{
    return led_state;
}

// Called when the device tree matches this driver's compatible string
static int myled_probe(struct platform_device *pdev)
{
    int ret;
    const char *label;

    printk(KERN_INFO "myled-dt: probe() called - matched by device tree!\n");

    if (of_property_read_string(pdev->dev.of_node, "label", &label) == 0)
        printk(KERN_INFO "myled-dt: label from DT = '%s'\n", label);

    myled_cdev.name           = "myled-dt";
    myled_cdev.max_brightness = 255;
    myled_cdev.brightness_set = myled_set;
    myled_cdev.brightness_get = myled_get;

    ret = led_classdev_register(&pdev->dev, &myled_cdev);
    if (ret) {
        printk(KERN_ERR "myled-dt: led register failed\n");
        return ret;
    }

    printk(KERN_INFO "myled-dt: registered /sys/class/leds/myled-dt/\n");
    return 0;
}

static void myled_remove(struct platform_device *pdev)
{
    led_classdev_unregister(&myled_cdev);
    printk(KERN_INFO "myled-dt: removed.\n");
}

// The link to the device tree: this driver handles "steve,myled" nodes
static const struct of_device_id myled_of_match[] = {
    { .compatible = "steve,myled", },
    { },
};
MODULE_DEVICE_TABLE(of, myled_of_match);

static struct platform_driver myled_driver = {
    .probe  = myled_probe,
    .remove = myled_remove,
    .driver = {
        .name           = "myled-dt",
        .of_match_table = myled_of_match,
    },
};

module_platform_driver(myled_driver);
