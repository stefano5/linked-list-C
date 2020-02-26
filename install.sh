if [ ! -f /usr/include/linked_list.c ]; then
    sudo cp linked_list.c /usr/include/
fi

gcc Example1/first_example.c -o first_example
gcc Example2/second_example.c -o second_example
