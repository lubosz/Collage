import operator
from urllib2 import urlopen
from HTMLParser import HTMLParser

tags = {}

class TagCounter(HTMLParser):
    def handle_starttag(self, tag, attrs):
        global tags
        if tag in tags:
            tags[tag] += 1
        else:
            tags[tag] = 1


websites = ['the-space-station.com',
            'uni-koblenz.de',
            'ubuntuusers.de',
            'google.de']


for website in websites:
    print "Parsing", website
    u = urlopen('http://' + website)
    p = TagCounter()
    p.feed(u.read())


print sorted(tags.iteritems(), key=operator.itemgetter(1))
