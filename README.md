
groyser
=======

This is a PHP-client for the electronic version of the גרויסער ווערטערבוך פון דער יידישער שפּראַך (Groyser verterbukh fun der idisher shprakh, by Yuda Yoffe and Yudl Mark) prepared by [Raphael A. Finkel](http://www.cs.uky.edu/~raphael/yiddish/searchGroys.cgi).

**Usage**

The query can be provided as a GET variable when run from a server:

   groyser.php?query=

Or as an argument when run from terminal:

   php groyser.php --query=

You will most likely use the client from within [GoldenDict](http://goldendict.org/). To add it as a custom dictionary: go to Sources→Dictionaries→Programs and click "Add". Choose "html" as the type, "groyser" as the name and paths to your php interpreter and the groyser.php script appended by the "query" template as the "Command line":

   /usr/bin/php /opt/groyser/groyser.php --query=%GDWORD%
