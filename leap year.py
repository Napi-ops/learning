# -*- coding:utf-8 -*-
year = 2000
while year > 1999 and year <2501:
    if (year % 4 == 0 and year % 100 != 0) or year %400 == 0:
        print(year,'是闰年')
    else:
        print(year,'不是闰年')
    year = year + 1