# 1. Last 3 lines of a file

#!/bin/bash
tail -n 3 $1

# 2. File size function

#!/bin/bash
size() {
    wc -c < $1
}
size $1

# 3. Ascending order of files in directory

#!/bin/bash
ls -1 $1 | sort

# 4. Word frequency in a file

#!/bin/bash
grep -o -i $2 $1 | wc -l