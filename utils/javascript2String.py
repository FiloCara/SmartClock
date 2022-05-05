
final_string = ""

with open("../webpage.html", 'r', encoding="utf8") as html:
    count = 0
    while True:
        count += 1
    
        # Get next line from file
        line = html.readline()
        # Remove empty spaces
        line = line.lstrip()

        final_string += f"ptr += " + f"{line}" + f'\n'
        # if line is empty
        # end of file is reached
        if not line:
            break

print(final_string)