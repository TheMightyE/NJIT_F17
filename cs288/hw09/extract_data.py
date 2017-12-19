#!/usr/bin/python
# CS288 Homework 9
# Read the skeleton code carefully and try to follow the structure
# You may modify the code but try to stay within the framework.

#import libxml2 """don't know what this is, didn't use it """
import sys
import os
import commands
import re
import sys

import MySQLdb

from xml.dom.minidom import parse, parseString

# for converting dict to xml 
from cStringIO import StringIO
from xml.parsers import expat

def get_elms_for_atr_val(tag,atr,val):
   elms = dom.getElementsByTagName(tag)
   elms = filter(lambda x: len(x.childNodes)==6,elms)
   
   return elms

# get all text recursively to the bottom
def get_text(e):
   lst=[]
   if e.nodeType in (3,4):
      data = e.data
      #data = replace_white_space(data)
      #data = replace_non_alpha_numeric(data)
      lst.append(data)
      
   else:
      for i in e.childNodes:
         lst = lst + get_text(i)

   return lst

def prettyfy_txt_lst(lst):
   l=[]
   l3=[]
   for i in lst:
      l2=[]
      # get number as int
      num = int(i[0])
      
      # seperate name and symbol
      s = i[1]
      titleSymb = s.split('(')
      title = str(replace_non_alpha_numeric(titleSymb[0]))
      title = replace_white_space(title)
      symb = str(replace_non_alpha_numeric(titleSymb[1]))
      symb = replace_white_space(symb)

      # get volume as int
      vol = replace_non_alpha_numeric(i[2])
      vol = vol.replace(' ', '')
      vol = int(vol)

      # get price as float
      price = float(i[3].replace('$', ''))
      
      # get change as float
      chnge = float(i[4])

      # get percent change as float
      percnt_chnge = float(i[5])

      l2.append(num)
      l2.append(symb)
      l2.append(title)
      l2.append(vol)
      l2.append(price)
      l2.append(chnge)
      l2.append(percnt_chnge)
      l.append(l2)

   l3 = (filter(lambda x: x[4]<10, l))
   for i in l3:
      print(i)
   return l

# replace whitespace chars
def replace_white_space(str):
   p = re.compile(r'\s+')
   new = p.sub(' ',str)   # a lot of \n\t\t\t\t\t\t
   return new.strip()

# replace but these chars including ':'
def replace_non_alpha_numeric(s):
   p = re.compile(r'[^a-zA-Z0-9:-]+')
   #p = re.compile(r'\W+') # replace whitespace chars
   new = p.sub(' ',s)
   return new.strip()

# convert to xhtml
# use: java -jar tagsoup-1.2.jar --files html_file
def html_to_xml(fn):
   commands.getoutput('java -jar tagsoup-1.2.1.jar --files ' + fn)
   xhtml_fn = fn.replace('.html', '.xhtml')
   return xhtml_fn

def extract_values(dm):
   #l = get_elms_for_atr_val('table','class','most_actives')
   l = get_elms_for_atr_val('tr','class','most_actives')
   final_lst=[]
   
   # skip the first element because its the column titles
   for i in l[1:]:
      lst = get_text(i)
      # filter lst to remove empty nodes (lst[1] and lst[3] are empty)
      filtered_lst = [lst[0]] + [lst[2]] + lst [4:]
      
      final_lst.append(filtered_lst)
      
   pretty_lst = prettyfy_txt_lst(final_lst)
   
   return pretty_lst

def lst_to_lst_of_dic(lst):
   l=[]
   for i in lst:
      dic = {}
      dic['num'] = i[0]
      dic['symb'] = i[1]
      dic['name'] = i[2]
      dic['vol'] = i[3]
      dic['price'] = i[4]
      dic['chng'] = i[5]
      dic['prcnt_chng'] = i[6]
      l.append(dic)

   return l
   

# mysql> describe most_active;
def insert_to_db(l,tbl):
   # ............
   global db
   db = MySQLdb.connect(host='localhost',user='cs288',passwd='zipp0',db='cs288')
   
   cur = db.cursor()

   cur.execute("USE cs288")

   # check if table already exists in db. If it doesn't, create it
   # drop query not working sometimes, don't know why
   query = "DROP TABLE IF EXISTS `{}`".format(tbl)
   cur.execute(query)
   
   # TODO: try/catch
   query = ("CREATE TABLE `{}` ("
            "`Number` int,"
            "`Symbol` varchar(10),"
            "`Name` varchar(100),"
            "`Volume` int,"
            "`Price` float (15,2),"
            "`Change` float (15,2),"
            "`Percent_change` float (15,2),"
            "PRIMARY KEY (`number`))"
            ).format(tbl)
   
   cur.execute(query)

   # insert to db
   for dic in l:
      query = "INSERT INTO `{}` VALUES ('{}','{}','{}','{}','{}','{}','{}')".format(tbl,
                                                                                    dic['num'],
                                                                                    dic['symb'],
                                                                                    dic['name'],
                                                                                    dic['vol'],
                                                                                    dic['price'],
                                                                                    dic['chng'],
                                                                                    dic['prcnt_chng'])
      
      cur.execute(query)
      
   #db.commit()
   #db.close()
   return cur

def select_from_db(cur,fn):
   query = "SELECT * from `{}`".format(fn)
   cur.execute(query)
   data = cur.fetchall()
   cur.close()

   db.commit()
   db.close()
   return data

def prntlst(lst):
   for i in lst:
      print i


def main():
   html_fn = sys.argv[1]
   xhtml_fn = html_to_xml(html_fn)
   # fn will be the table name in the db
   fn = xhtml_fn.replace('.xhtml','').split('/')[1]
   
   global dom
   dom = parse(xhtml_fn)
   
   lst = extract_values(dom)
   #prntlst(lst)
   lst = lst_to_lst_of_dic(lst)
   #prntlst(lst)
   
   # make sure your mysql server is up and running
   
   cursor = insert_to_db(lst,fn) # fn = table name for mysql
   l = select_from_db(cursor,fn) # display the table on the screen
   
   # make sure the apache web server is up and running
   # write a php script to display the table(s) on your browser
   
   return 0
# end of main()

if __name__ == "__main__":
    main()

# end of hw9.py
