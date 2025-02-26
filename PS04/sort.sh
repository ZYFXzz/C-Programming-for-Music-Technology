SOURCE_FILES="sort_main.c sort_struct.c" 
OUTPUT_EXECUTABLE="sort"         
INPUT_FILE="name_id.csv"                   
OUTPUT_FILE="output.csv"                 
SORT_KEY="id"     # first， last  , id                   

# compile
echo "gcc running"
gcc $SOURCE_FILES -o $OUTPUT_EXECUTABLE
if [ $? -ne 0 ]; then
    echo "compile failed"
    exit 1
fi
echo "compiled: $OUTPUT_EXECUTABLE"

# running
echo "programme running..."
./$OUTPUT_EXECUTABLE $SORT_KEY $INPUT_FILE $OUTPUT_FILE
if [ $? -ne 0 ]; then
    echo "error！"
    exit 1
fi
echo "completed: $OUTPUT_FILE"