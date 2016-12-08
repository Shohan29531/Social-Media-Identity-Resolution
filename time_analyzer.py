twitter="Fri Nov 04 16:19:44 +0000 2016"

disqus="2016-05-19T16:46:11"


def time_analyzer_disqus(s):
    



def time_analyzer_twitter(s):
    li =[]
    li=str.split(" ")

    day_of_week=li[0]
    month=li[1]
    day=li[2]

    li2=li[3].split(":")

    hh=li2[0]
    mm=li2[1]
    ss=li2[2]

    timezone=li[4]
    year=li[5]
    
    final=[]
    final.append(day_of_week)
    final.append(month)
    final.append(day)
    final.append(hh)
    final.append(mm)
    final.append(ss)
    final.append(timezone)
    final.append(year)

    print(final)
    
time_analyzer_twitter(twitter)









