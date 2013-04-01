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
    'default'      : None,
    'last'         : False
}
    
import ConfigParser
config = ConfigParser.RawConfigParser()
config.read(ini_file)

sections = config.sections()

""" parse an ini directive

this function returns a list of table fields in the order of declaration.

every field can have the attributes defined in db_field. 
 
"""
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
                current_db_field['last'] = True
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

""" generate sql for a table 

the input parameter is a list of db_field
"""
def generate_table_sql(e):
    buffer = ""
    if (e['size'] > 0 and e['type'] != "integer"):
        buffer += "\t\"%s\" %s(%s) " % (e['name'], e['type'].upper(), e['size'])
    else:
        buffer += "\t\"%s\" %s " % (e['name'], e['type'].upper())
    
    if e['primary_key']:
        buffer += "PRIMARY KEY "
    
    if e['autoincrement']:
        buffer += "AUTOINCREMENT "
    
    if not e['null']:
        buffer += "NOT NULL "
    
    if e['default']:
        buffer += "DEFAULT (%s) " % e['default']
    
    if buffer.endswith(" "): 
        buffer = buffer[:-1]
    
    if e['last']:
        buffer += ","
    return buffer + "\n"

# parse ini file and generate:
#    schema.sql
fp_sql = open("schema.sql", "w")
for s in sections:
    fp_sql.write("CREATE TABLE \"%s\" (\n" % s)
    r = parse_tbl(s)
    for tbl in r:
        fp_sql.write(generate_table_sql(tbl))
    fp_sql.write(");\n")
