#!/usr/bin/env python

import sys 

ini_file = 'schema.ini'
db_field_types = ["integer", "varchar", "text"]

"""
class db_field(object):
    name = None
    type = None
    size = None
    primary_key = False
    autoincrement = False
    null = True
    
    def set_(self, name, value):
"""

db_field = {
    'name'         : None,
    'type'         : None,
    'size'         : None,
    'primary_key'  : False,
    'autoincrement': False,
    'null'         : True,
    'default'      : None
}
    
import ConfigParser
config = ConfigParser.RawConfigParser()
config.read(ini_file)

sections = config.sections()

def parse_tbl(table_name):
    db_current_table = []
    last_prefix = None
    current_db_field = None
    task = config.items(table_name)
    for (name, value) in task:
        pos = name.find('.') # found
        if (pos == -1):
            print "Error while parsing database definition '%s', key: %s, value: %s" % (ini_file, name, value)
            sys.exit(1)
        prefix = name[0:pos]
        directive = name[pos+1:]
        
        if (prefix != last_prefix):
            if current_db_field != None:
                db_current_table.append(current_db_field)
                #print current_db_field
            last_prefix = prefix
            current_db_field = dict(db_field)
            current_db_field['name'] = prefix
        
        # check if this is a valid type
        if directive == "type":
            try:
                db_field_types.index(value)
            except:
                print "Type: '%s' unknown" % value
                sys.exit(2)
        
        #current_db_field.setattr(directive, value)
        if (value == "true"):
            value = True
        if (value == "false"):
            value = False
        if (value == "null"):
            value = None
        if (directive == "size"):
            value = int(value)
        
        # append to result
        current_db_field[directive] = value
        
        #print pos, prefix, directive, value
    
    db_current_table.append(current_db_field)
    return db_current_table

def generate_sql(e):
    if (e['size'] > 0):
        print "\t\"%s\" %s(%s)" % (e['name'], e['type'].upper(), e['size']),
    else:
        print "\t\"%s\" %s" % (e['name'], e['type'].upper()),
    
    if e['primary_key']:
        print "PRIMARY KEY",
    
    if e['autoincrement']:
        print "AUTOINCREMENT",
    
    if not e['null']:
        print "NOT NULL",
    
    if e['default']:
        print "DEFAULT (%s)" % e['default'],
    
    print ""

for s in sections:
    print "CREATE TABLE \"%s\" (" % s
    r = parse_tbl(s)
    for e in r:
        generate_sql(e) 
    print ");"
