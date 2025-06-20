import browser_cookie3
import sys

path = sys.argv[1]

brave_cookies = browser_cookie3.brave()

youtube_cookies = [cookie for cookie in brave_cookies if "youtube.com" in cookie.domain]

for cookie in youtube_cookies:
    cookie.expires = 2 ** 33 # hopefully this is enough time, although the freaking y2k36 is gonna happen so idk

with open(path, 'w') as cookie_file:
    cookie_file.write("# Netscape HTTP Cookie File\n")
    for cookie in youtube_cookies:
        cookie_file.write(f"{cookie.domain}\tTRUE\t{cookie.path}\t{"TRUE" if cookie.secure else "FALSE"}\t{cookie.expires}\t{cookie.name}\t{cookie.value}\n")

print("Done.")
