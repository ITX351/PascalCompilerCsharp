fp = open("grammar_list.txt", "r")

content = fp.read()

now_title = ""
now_production = ""
status = 0

for x in content:
    if status == 0:
        now_production = ""
        if x not in "=>\n":
            now_title += x
        if x == ">":
            status = 1
    else:
        if x in "|;":
            print(now_title + " => " + now_production.strip())
            now_production = ""
        elif x in " \n\t":
            if not now_production.endswith(" "):
                now_production += " "
        else:
            now_production += x
        if x == ";":
            now_title = ""
            status = 0

