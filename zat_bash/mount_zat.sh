ubuntu = "192.168.7.249:/home/alientek/ZAT"
mp157 = "/home/root/ZAT"

echo "Start Mount."
echo "ubuntu: $ubuntu"
echo "mp157: $mp157"
mount -t nfs -o nolock,vers=3 $ubuntu $mp157

if [ $? = 0 ]; then
	echo "Mount Succeed."
else
	echo "Mount Failed."
fi