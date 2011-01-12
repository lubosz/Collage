import operator
from urllib2 import urlopen
from HTMLParser import HTMLParser


class LevelConstructor(HTMLParser):
    def __init__(self, **kwargs):
        #super(LevelConstructor, self).__init__(**kwargs)
        HTMLParser.__init__(self, **kwargs)
        self.level = []
        self.ignored_tags = ['a', 'li']

    def prev_height(self):
        try:
            prev_height = self.level[-1]
        except IndexError:
            # First tag
            prev_height = -1
        return prev_height

    def handle_starttag(self, tag, attrs):
        if tag in self.ignored_tags:
            return
        factor = 1 if tag != 'embed' else 5
        prev_height = self.prev_height()
        for i in range(factor):
            self.level.append(prev_height + 1)
            print (prev_height + 1) * '.', tag

    def handle_endtag(self, tag):
        if tag in self.ignored_tags:
            return
        self.level.append(self.prev_height() - 1)
        print (self.prev_height() - 1) * '.', tag


u = urlopen('http://google.com')
p = LevelConstructor()
p.feed(u.read())

print p.level

