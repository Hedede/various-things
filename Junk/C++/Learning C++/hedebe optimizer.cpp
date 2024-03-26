int findcar(char findname[20]) {
    int lenght;
    lenght = strlen(findname);
    int win = 0;
    for(int i = 0; i <= 211; i++) {
        int g = 1;
        for(int b = 0; g == 1 && b < lenght; b++) {
            if(findname[b] == vehnames[i][b]) {
                g = 1;
            } else {
                g = 0;
            }
        }
        if(g == 1) {
            omgochnulso = i + 400;
            cout << "car id:" << omgochnulso << ", car name: \"" << vehnames[i] << "\"\n";
            win = 1;
        }
    }
    if(win == 1) {
        cout << "> Џ®Ё§Ј §®ўҐаи®­! ¤®ў®«Ґ­?\n";
    } else {
        cout << "> “ ¬Ґ­п ­Ґв ¬®дл­®Є б в ЄЁ¬Ё Ё­вҐаҐб­л¬Ё ­ §ў ­Ёп¬Ё((\n";
    }
    return 1;
}

void findcar2(char *findname) {
    int length = strlen(findname);
	int i = 0, b = 0;
	bool win = false;
    while(i++ < 211) {
		b = 0;
        while(*(findname+b) && (*(findname+b) == *(vehnames[i]+b))) b++;
		if(b == length) {
            cout << "car id:" << i + 400 << ", car name: \"" << vehnames[i] << "\"\n";
            win = true;
        }
    }
    cout << (win ? "> dovolen, kukufcheg?\n" : "> nenashol mya tojego ucoza\n");
}
    
void findcar3(char findname[20]) {
    int len = strlen(findname);
	bool win = false;
	int i = 0;
    while(i++ < 211) {
        if(!strncmp(findname, vehnames[i], len)) {
            cout << "car id:" << i + 400 << ", car name: \"" << vehnames[i] << "\"\n";
            win = true;
        }
    }
    cout << (win ? "> dovolen, kukufcheg?\n" : "> nenashol mya tojego ucoza\n");
}  