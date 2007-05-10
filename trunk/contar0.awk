#!/usr/bin/awk -f
BEGIN{RS=""}
{print "--------------------------------------"
print $0
}
END{ print NR }
