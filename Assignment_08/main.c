#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Louis Solofrizzo <louis@ne02ptzero.me>, eboris");
MODULE_DESCRIPTION("Useless module");

static ssize_t myfd_read(struct file *fp, char __user *user,
				size_t size, loff_t *offs);
static ssize_t myfd_write(struct file *fp, const char __user *user,
				size_t size, loff_t *offs);
static void __exit myfd_cleanup(void);

static struct file_operations myfd_fops = {
	.owner = THIS_MODULE,
	.read = &myfd_read,
	.write = &myfd_write
};

static struct miscdevice myfd_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "reverse",
	.fops = &myfd_fops
};

char str[PAGE_SIZE];

static int __init myfd_init(void)
{
	int retval;

	retval = misc_register(&myfd_device);
	return (retval);
}

static void __exit myfd_cleanup(void)
{
	misc_deregister(&myfd_device);
}

ssize_t myfd_read(struct file *fp, char __user *user, size_t size,
			loff_t *offs)
{
	size_t t, i, len;
	char *tmp;
	int res;

	len = strlen(str);
	if (len <= 0)
		myfd_cleanup();
	tmp = kmalloc(sizeof(char) * PAGE_SIZE * 2, GFP_KERNEL);
	for (t = len, i = 0; t >= 0; t--, i++) {
		tmp[i] = str[t];
	}
	tmp[i] = '\0';
	res = simple_read_from_buffer(user, size, offs, tmp, i);
	kfree(tmp);
	(void)*fp;
	return (res);
}

ssize_t myfd_write(struct file *fp, const char __user *user, size_t size,
			loff_t *offs)
{
	ssize_t res;

	res = 0;
	res = simple_write_to_buffer(str, size, offs, user, size) + 1;
	str[size + 1] = '\0';
	(void)*fp;
	return res;
}

module_init(myfd_init);
module_exit(myfd_cleanup);
