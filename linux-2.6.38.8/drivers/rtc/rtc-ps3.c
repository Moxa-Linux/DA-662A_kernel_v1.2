/*
 * PS3 RTC Driver
 *
 * Copyright 2009 Sony Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/rtc.h>

#include <asm/lv1call.h>
#include <asm/ps3.h>


static u64 read_rtc(void)
{
	int result;
	u64 rtc_val;
	u64 tb_val;

	result = lv1_get_rtc(&rtc_val, &tb_val);
	BUG_ON(result);

	return rtc_val;
}

static int moxaart_get_time(struct device *dev, struct rtc_time *tm)
{
	rtc_time_to_tm(read_rtc() + moxaart_os_area_get_rtc_diff(), tm);
	return rtc_valid_tm(tm);
}

static int moxaart_set_time(struct device *dev, struct rtc_time *tm)
{
	unsigned long now;

	rtc_tm_to_time(tm, &now);
	moxaart_os_area_set_rtc_diff(now - read_rtc());
	return 0;
}

static const struct rtc_class_ops moxaart_rtc_ops = {
	.read_time = moxaart_get_time,
	.set_time = moxaart_set_time,
};

static int __init moxaart_rtc_probe(struct platform_device *dev)
{
	struct rtc_device *rtc;

	rtc = rtc_device_register("rtc-ps3", &dev->dev, &moxaart_rtc_ops,
				  THIS_MODULE);
	if (IS_ERR(rtc))
		return PTR_ERR(rtc);

	platform_set_drvdata(dev, rtc);
	return 0;
}

static int __exit moxaart_rtc_remove(struct platform_device *dev)
{
	rtc_device_unregister(platform_get_drvdata(dev));
	return 0;
}

static struct platform_driver moxaart_rtc_driver = {
	.driver = {
		.name = "rtc-ps3",
		.owner = THIS_MODULE,
	},
	.remove = __exit_p(moxaart_rtc_remove),
};

static int __init moxaart_rtc_init(void)
{
	return platform_driver_probe(&moxaart_rtc_driver, moxaart_rtc_probe);
}

static void __exit moxaart_rtc_fini(void)
{
	platform_driver_unregister(&moxaart_rtc_driver);
}

module_init(moxaart_rtc_init);
module_exit(moxaart_rtc_fini);

MODULE_AUTHOR("Sony Corporation");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("ps3 RTC driver");
MODULE_ALIAS("platform:rtc-ps3");
