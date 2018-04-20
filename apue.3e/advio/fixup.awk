#!/usr/bin/awk -f
BEGIN		{
			ready = 0
		}
/^translate/	{
			ready = 1
			next
		}
/^{/		{
			next
		}
/^}/		{
			ready = 0
			next
		}
		{
			if (ready)
				print $0
		}
