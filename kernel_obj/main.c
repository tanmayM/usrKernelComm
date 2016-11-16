#include <linux/kernel.h>
#include <linux/inetdevice.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/skbuff.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/moduleparam.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netdevice.h>
#include <net/sock.h>
#include <net/ip.h>
#include <net/tcp.h>
#include <linux/time.h>
#include <asm/uaccess.h>
#include <linux/string.h>
#include <linux/types.h>

#include <linux/list.h>
#include <linux/hash.h>
#include <linux/jiffies.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tanmay");
MODULE_DESCRIPTION("user-kernel space comm");

#define MY_ID 128
#define MY_ID_MAX 129

static int recv_cmd(struct sock *sk,int cmd, void __user *user,unsigned int len)
{
    char str[20];
    printk(KERN_INFO "in recv_cmd\n");
    if(0!= copy_from_user((char*)str, user, 20))
    {
        printk(KERN_INFO "error: can not copy data ******from****** userspace\n");
        return -1;
    }
    printk(KERN_INFO "Received %s", str);
    return 0;
}

static int send_cmd(struct sock *sk,int cmd, void __user *user,int *len)
{
    char * str = "def";
    printk(KERN_INFO "send_cmd enter \n");
    if(0!= copy_to_user(user,(char*)str, strlen(str)))
    {
        printk(KERN_INFO "error: can't copy data to userspace\n");
        return -1;
    }
    printk(KERN_INFO "sent following string to userspace : %s\n", str);
    return 0;
}

struct nf_sockopt_ops my_sockops = {
    .pf = PF_INET,
    .set_optmin = MY_ID,
    .set_optmax = MY_ID_MAX,
    .set = recv_cmd,
    .get_optmin = MY_ID,
    .get_optmax = MY_ID_MAX,
    .get = send_cmd
};


static unsigned int myhook_func(
        const struct nf_hook_ops *ops,
        //unsigned int hooknum, 
        struct sk_buff *skb, const struct net_device *in, const struct net_device *out, int (*okfn)(struct sk_buff *))

{
    return NF_ACCEPT;
}


static struct nf_hook_ops nfho={

    .hook           = myhook_func,
    .owner          = THIS_MODULE,
    .pf             = PF_INET,
    .hooknum        = NF_INET_POST_ROUTING,
    .priority       = NF_IP_PRI_FIRST,
};

static int __init init_func(void)
{

    printk(KERN_INFO "In sockop ko init\n");
    nf_register_sockopt(&my_sockops);

    return nf_register_hook(&nfho);
}

static void __exit exit_func(void)
{
    printk(KERN_INFO "In sockop ko exit\n");
    nf_unregister_hook(&nfho);
    nf_unregister_sockopt(&my_sockops);
}


module_init(init_func);
module_exit(exit_func);
