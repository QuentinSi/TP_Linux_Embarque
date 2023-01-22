#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/timer.h>
#include <linux/proc_fs.h>

#define PROC_PARENT_NAME	"ensea"
#define PROC_NAME1		"speed"
#define PROC_NAME2 "dir"
struct proc_dir_entry* parent;

static unsigned int blink_frequency = 50;
unsigned char led_state = 0x01;
static struct timer_list tim1;
static unsigned int pattern=1;
static unsigned int sens=1;
// Prototypes
static int leds_probe(struct platform_device *pdev);
static int leds_remove(struct platform_device *pdev);
static ssize_t leds_read(struct file *file, char *buffer, size_t len, loff_t *offset);
static ssize_t leds_write(struct file *file, const char *buffer, size_t len, loff_t *offset);
static ssize_t read_speed(struct file *file, char __user * buffer, size_t count, loff_t * ppos);
static ssize_t read_pattern(struct file *file, char __user *buffer, size_t count, loff_t *ppos);
static ssize_t write_pattern(struct file *file, const char __user * buffer, size_t count, loff_t * ppos);
static ssize_t read_sens(struct file *file, char __user * buffer, size_t count, loff_t * ppos);
static ssize_t write_sens(struct file *file, const char __user * buffer, size_t count, loff_t * ppos);


    module_param(blink_frequency, uint, S_IRUGO);
    MODULE_PARM_DESC(blink_frequency, "LED blink frequency");

// An instance of this structure will be created for every ensea_led IP in the system
struct ensea_leds_dev {
    struct miscdevice miscdev;
    void __iomem *regs;
    u8 leds_value;
};

// Specify which device tree devices this driver supports
static struct of_device_id ensea_leds_dt_ids[] = {
    {
        .compatible = "dev,ensea"
    },
    { /* end of table */ }
};

// Inform the kernel about the devices this driver supports
MODULE_DEVICE_TABLE(of, ensea_leds_dt_ids);

// Data structure that links the probe and remove functions with our driver
static struct platform_driver leds_platform = {
    .probe = leds_probe,
    .remove = leds_remove,
    .driver = {
        .name = "Ensea LEDs Driver",
        .owner = THIS_MODULE,
        .of_match_table = ensea_leds_dt_ids
    }
};

// The file operations that can be performed on the ensea_leds character file
static const struct file_operations ensea_leds_fops = {
    .owner = THIS_MODULE,
    .read = leds_read,
    .write = leds_write
};

struct file_operations proc_fops_read = {
	.read=read_speed,
};

struct file_operations proc_fops_read_sens = {
	.read=read_sens,
    .write=write_sens
};
struct file_operations proc_fops_dev = {
	.read=read_pattern,
    .write=write_pattern
};

// Called when the driver is installed
static int leds_init(void)
{
    int ret_val = 0;
    pr_info("Initializing the Ensea LEDs module\n");

    // Register our driver with the "Platform Driver" bus
    ret_val = platform_driver_register(&leds_platform);
    if(ret_val != 0) {
        pr_err("platform_driver_register returned %d\n", ret_val);
        return ret_val;
    }

    parent = proc_mkdir(PROC_PARENT_NAME, NULL);
	proc_create(PROC_NAME1,0,parent,&proc_fops_read);
    proc_create(PROC_NAME2,0,parent,&proc_fops_read_sens);
    int k=register_chrdev(10,"ensea_leds",&proc_fops_dev);
    printk(KERN_INFO "la valeur de mon dev est : %d\n",k);
    pr_info("Ensea LEDs module successfully initialized!\n");

    return 0;
}
struct platform_device *pdev1;
static void tim1_callback(unsigned long data) {
	struct ensea_leds_dev *dev = (struct ensea_leds_dev*)platform_get_drvdata(pdev1);
	if(pattern==1){
        led_state ^= 0xFF;
        iowrite32(led_state,dev->regs);
        mod_timer(&tim1, jiffies + blink_frequency);
    } else if(sens==1) {
        if(led_state!=0){
        led_state = led_state>>1;
        iowrite32(led_state,dev->regs);	
        mod_timer(&tim1, jiffies + blink_frequency);
        }
        else{
            led_state=0x80;
            iowrite32(led_state,dev->regs);	
            mod_timer(&tim1, jiffies + blink_frequency);
        }
    } else {
        if(led_state<0x80){
            led_state = led_state<<1;
            iowrite32(led_state,dev->regs);	
            mod_timer(&tim1, jiffies + blink_frequency);
        }
        else{
            led_state=0x01;
            iowrite32(led_state,dev->regs);	
            mod_timer(&tim1, jiffies + blink_frequency);
        }
    }

}

//Cette fonction est appelée lorsqu'un utilisateur lit le fichier de procédure associé à la vitesse de clignotement des LEDs. Il copie la valeur actuelle de la fréquence de clignotement dans un tampon fourni par l'utilisateur et renvoie le nombre d'octets copiés.
static ssize_t read_speed(struct file *file, char __user * buffer, size_t count, loff_t * ppos){
    char message[100];
    int errno=0;
    int copy;
    sprintf(message,"%d\n",blink_frequency);
    size_t datalen=strlen(message);
    if(*ppos>=datalen){
        return 0;
    }
    if(count>datalen-*ppos){
        count=datalen-*ppos;
    }
    if((copy=copy_to_user(buffer,message,count)))
        errno=-EFAULT;
    printk(KERN_INFO "message read, %d, %p\n",copy,buffer);
    *ppos+=count;
    return count;
}

//Cette fonction est appelée lorsqu'un utilisateur lit le fichier de procédure associé au motif des LEDs. Il copie la valeur actuelle du motif dans un tampon fourni par l'utilisateur et renvoie le nombre d'octets copiés.
static ssize_t read_pattern(struct file *file, char __user *buffer, size_t count, loff_t *ppos)
{
   	printk(KERN_INFO "je suis dans le patter");
    struct ensea_leds_dev *dev = (struct ensea_leds_dev*)platform_get_drvdata(pdev1);
    char message[100];
    int errno = 0;
    int copy;
    //unsigned int pattern;

    //pattern = ioread32(dev->regs);

    sprintf(message, "%d\n", pattern);
    size_t datalen = strlen(message);

    if (*ppos >= datalen)
    {
        return 0;
    }
    if (count > datalen - *ppos)
    {
        count = datalen - *ppos;
    }
    if ((copy = copy_to_user(buffer, message, count)))
    {
        errno = -EFAULT;
    }

    printk(KERN_INFO "pattern read, %d, %p\n", copy, buffer);
    *ppos += count;
    return count;
}

//Cette fonction est appelée lorsqu'un utilisateur écrit dans le fichier de procédure associé au motif des LEDs. Il met à jour le patern en fonction des données fournies par l'utilisateur et renvoie le nombre d'octets utilisés pour mettre à jour le motif
static ssize_t write_pattern(struct file *file, const char __user * buffer, size_t count, loff_t * ppos)
{
    char message[100];
    int errno = 0;
    int copy;
    if (count >= sizeof(message)) {
        count = sizeof(message) - 1;
    }
    if ((copy = copy_from_user(message, buffer, count))) {
        errno = -EFAULT;
    }
    message[count] = '\0';

    // convert the message buffer to an integer
    int value = simple_strtol(message, NULL, 10);
    pattern = value;

    printk(KERN_INFO "message written, %d, %p\n", copy, buffer);
    *ppos += count;
    return count;
}

//Cette fonction est appelée lorsqu'un utilisateur lit le fichier de procédure associé au sens de rotation des LEDs. Il copie la valeur actuelle du sens de rotation dans un tampon fourni par
static ssize_t read_sens(struct file *file, char __user * buffer, size_t count, loff_t * ppos){
    char message[100];
    int errno=0;
    int copy;
    sprintf(message,"%d\n",sens);
    size_t datalen=strlen(message);
    if(*ppos>=datalen){
        return 0;
    }
    if(count>datalen-*ppos){
        count=datalen-*ppos;
    }
    if((copy=copy_to_user(buffer,message,count)))
        errno=-EFAULT;
    printk(KERN_INFO "message read, %d, %p\n",copy,buffer);
    *ppos+=count;
    return count;
}
static ssize_t write_sens(struct file *file, const char __user * buffer, size_t count, loff_t * ppos)
{
    char message[100];
    int errno = 0;
    int copy;
    if (count >= sizeof(message)) {
        count = sizeof(message) - 1;
    }
    if ((copy = copy_from_user(message, buffer, count))) {
        errno = -EFAULT;
    }
    message[count] = '\0';

    // convert the message buffer to an integer
    int value = simple_strtol(message, NULL, 10);
    sens = value;

    printk(KERN_INFO "message written, %d, %p\n", copy, buffer);
    *ppos += count;
    return count;
}

// Cette fonction est appelée lorsque le pilote est associé à un périphérique spécifique. Il initialise les ressources matérielles nécessaires pour le périphérique et renvoie 0 en cas de succès, un code d'erreur négatif en cas d'échec.
static int leds_probe(struct platform_device *pdev)
{
    int ret_val = -EBUSY;
    struct ensea_leds_dev *dev;
    struct resource *r = 0;

    pr_info("leds_probe enter\n");

    // Get the memory resources for this LED device
    r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if(r == NULL) {
        pr_err("IORESOURCE_MEM (register space) does not exist\n");
        goto bad_exit_return;
    }

    // Create structure to hold device-specific information (like the registers)
    dev = devm_kzalloc(&pdev->dev, sizeof(struct ensea_leds_dev), GFP_KERNEL);
    pdev1=pdev;
    // Both request and ioremap a memory region
    // This makes sure nobody else can grab this memory region
    // as well as moving it into our address space so we can actually use it
    dev->regs = devm_ioremap_resource(&pdev->dev, r);
    if(IS_ERR(dev->regs))
        goto bad_ioremap;

    // Turn the LEDs on (access the 0th register in the ensea LEDs module)
    dev->leds_value = 0xFF;
    iowrite32(dev->leds_value, dev->regs);

    //POur le timer
    setup_timer(&tim1, tim1_callback, 0);
	mod_timer(&tim1, jiffies + blink_frequency);

    // Initialize the misc device (this is used to create a character file in userspace)
    dev->miscdev.minor = MISC_DYNAMIC_MINOR;    // Dynamically choose a minor number
    dev->miscdev.name = "ensea_leds";
    dev->miscdev.fops = &ensea_leds_fops;

    ret_val = misc_register(&dev->miscdev);
    if(ret_val != 0) {
        pr_info("Couldn't register misc device :(");
        goto bad_exit_return;
    }

    // Give a pointer to the instance-specific data to the generic platform_device structure
    // so we can access this data later on (for instance, in the read and write functions)
    platform_set_drvdata(pdev, (void*)dev);

    pr_info("leds_probe exit\n");

    return 0;

bad_ioremap:
   ret_val = PTR_ERR(dev->regs);
bad_exit_return:
    pr_info("leds_probe bad exit :(\n");
    return ret_val;
}

// This function gets called whenever a read operation occurs on one of the character files
static ssize_t leds_read(struct file *file, char *buffer, size_t len, loff_t *offset)
{
    int success = 0;

    /*
    * Get the ensea_leds_dev structure out of the miscdevice structure.
    *
    * Remember, the Misc subsystem has a default "open" function that will set
    * "file"s private data to the appropriate miscdevice structure. We then use the
    * container_of macro to get the structure that miscdevice is stored inside of (which
    * is our ensea_leds_dev structure that has the current led value).
    *
    * For more info on how container_of works, check out:
    * http://linuxwell.com/2012/11/10/magical-container_of-macro/
    */
    struct ensea_leds_dev *dev = container_of(file->private_data, struct ensea_leds_dev, miscdev);

    // Give the user the current led value
    success = copy_to_user(buffer, &dev->leds_value, sizeof(dev->leds_value));

    // If we failed to copy the value to userspace, display an error message
    if(success != 0) {
        pr_info("Failed to return current led value to userspace\n");
        return -EFAULT; // Bad address error value. It's likely that "buffer" doesn't point to a good address
    }

    return 0; // "0" indicates End of File, aka, it tells the user process to stop reading
}

//Cette fonction est appelée lorsqu'un utilisateur écrit dans le fichier de caractères associé au périphérique. Elle met à jour l'état des LEDs en fonction des données fournies par l'utilisateur et renvoie le nombre d'octets utilisés pour mettre à jour l'état.
static ssize_t leds_write(struct file *file, const char *buffer, size_t len, loff_t *offset)
{
    int success = 0;

    /*
    * Get the ensea_leds_dev structure out of the miscdevice structure.
    *
    * Remember, the Misc subsystem has a default "open" function that will set
    * "file"s private data to the appropriate miscdevice structure. We then use the
    * container_of macro to get the structure that miscdevice is stored inside of (which
    * is our ensea_leds_dev structure that has the current led value).
    *
    * For more info on how container_of works, check out:
    * http://linuxwell.com/2012/11/10/magical-container_of-macro/
    */
    struct ensea_leds_dev *dev = container_of(file->private_data, struct ensea_leds_dev, miscdev);

    // Get the new led value (this is just the first byte of the given data)
    success = copy_from_user(&dev->leds_value, buffer, sizeof(dev->leds_value));

    int blink= buffer[0] == '1';

    if(blink){
        dev->leds_value== 0xff;
    }
    else{
        dev->leds_value=buffer[0];
    }
    // If we failed to copy the value from userspace, display an error message
    if(success != 0) {
        pr_info("Failed to read led value from userspace\n");
        return -EFAULT; // Bad address error value. It's likely that "buffer" doesn't point to a good address
    } else {
        // We read the data correctly, so update the LEDs
        iowrite32(dev->leds_value, dev->regs);
    }

    // Tell the user process that we wrote every byte they sent
    // (even if we only wrote the first value, this will ensure they don't try to re-write their data)
    return len;
}

// Cette fonction est appelée lorsque le pilote est désassocié d'un périphérique spécifique. Il libère les ressources matérielles allouées pour le périphérique et renvoie 0 en cas de succès, un code d'erreur négatif en cas d'échec.
static int leds_remove(struct platform_device *pdev)
{
    // Grab the instance-specific information out of the platform device
    struct ensea_leds_dev *dev = (struct ensea_leds_dev*)platform_get_drvdata(pdev);
    del_timer(&tim1);
    remove_proc_entry(PROC_NAME2, parent);
    remove_proc_entry(PROC_NAME1, parent);
	remove_proc_entry(PROC_PARENT_NAME, NULL);
    unregister_chrdev(10,"ensea_leds");

    pr_info("leds_remove enter\n");

    // Turn the LEDs off
    iowrite32(0x00, dev->regs);

    // Unregister the character file (remove it from /dev)
    misc_deregister(&dev->miscdev);

    pr_info("leds_remove exit\n");

    return 0;
}

// Called when the driver is removed
static void leds_exit(void)
{
    pr_info("Ensea LEDs module exit\n");

    // Unregister our driver from the "Platform Driver" bus
    // This will cause "leds_remove" to be called for each connected device
    platform_driver_unregister(&leds_platform);

    pr_info("Ensea LEDs module successfully unregistered\n");
}

// Tell the kernel which functions are the initialization and exit functions
module_init(leds_init);
module_exit(leds_exit);

// Define information about this kernel module
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Quentin et Ricardo");
MODULE_DESCRIPTION("Exposes a character device to user space that lets users turn LEDs on and off");
MODULE_VERSION("1.0");
