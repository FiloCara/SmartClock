
final_string = ''

def adjust_quotation_marks(line):
    i = 0
    while i < len(line):
        if line[i] == "\"":
            line = line[:i] + "\\" + line[i:]
            i+=2
        else:
            i+=1 

    return line

with open("webpage.html", 'r', encoding="utf8") as html:
    count = 0
    for line in html.readlines():
        
        # Remove whitespaces
        line = line.lstrip().strip()

        line = adjust_quotation_marks(line)

        if (line != "") or (line.isspace() == False):
            final_string += 'ptr += ' + '\"' + f'{line}' + '\"' + ';' + '\n'

print(final_string)