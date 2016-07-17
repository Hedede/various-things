#include <iostream>

int main () {
    int ia;
    int i;
    count:
    i = (ia + ia) % 94;
    printf("%d\n",ia);
    printf("%d\n",i);
    printf("====\n");
    std::cin >> ia;
    printf("---\n");
    goto count;
    return 0;   
}
