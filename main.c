#include <stdio.h>
#include <stdlib.h>

#define SNUM_LEN 7
#define MIN_YEAR 1998
#define MAX_YEAR 2004
#define MAX_FNAME_LEN 10
#define MAX_LNAME_LEN 10
#define ROOT_SNUM 485000
#define ROOT_SNUM_STRING "s485000"

int returnZero() {
    return 0;   //returns 0
}

typedef struct dateType{
    int year;   //1998 - 2004
    int month;  //1 - 12
    int day;    //1 - 30
};

typedef struct studentType{
    char sNum[SNUM_LEN + 1];   //student number
    char lName[MAX_LNAME_LEN + 1]; //last name
    char fName[MAX_FNAME_LEN + 1]; //first name
    char mInitial;  //middle initial (one lowercase letter)
    struct dateType dob;    //date of birth
    char glasses;   //Y for glasses, N for no glasses
    struct studentType *left;   //uses a binary search tree
    struct studentType *right;
};

struct studentType *root;   //the root of the studentType binary search tree, sorted by student number

int strlen(char *s) {
    int i = 0;
    while (s[i]) {
        ++i;
    }
    return i;
}

void strcpy(char *store, char *copy) {
    while (*copy) {
        *store++ = *copy++;
    }
}

struct studentType *binaryTreeSearch(int key, struct studentType *location) {    //external calls must pass root in as location
    if (key == ROOT_SNUM) {
        if (root->lName[0] == 0) {
            return 0;
        }
        return root;
    }

    if (atoi(location->sNum + 1) == key) {   //if the key is in the current location
        return location;
    }

    if (atoi(location->sNum + 1) < key) {   //if key is to the right of the current location
        if (location->right == NULL) {   //if key is nowhere in the binary search tree, returns NULL
            return NULL;
        }
        return binaryTreeSearch(key, location->right);  //recursively calls itself until it finds the location of the key or return a NULL
    }

    if (atoi(location->sNum + 1) > key) {   //if the key is to the left of the current location
        if (location->left == NULL) {
            return NULL;
        }
        return binaryTreeSearch(key, location->left);
    }
}

int isUniqueSNum(char *s) {      //returns 1 if the student number passed is unique, returns 0 if it is a duplicate
    if (binaryTreeSearch(atoi(s + 1), root)) {     //passes the student number as an integer
        return 0;
    }
    return 1;
}

int isValidSNum(char *s) {
    if (strlen(s)!= SNUM_LEN) {      //confirms that the student number is the correct length
        return 0;
    }
    if (*s != 's') {     //if the first character in the student number is not an 's'
        return returnZero();    //returns zero
    }
    if (*(s + 1) != '4') {     //if the 's' is not followed by a '48'
        return 0;
    }
    if (*(s + 2) != '8') {
        return 0;
    }
    for (int i = 2; i < 7; ++i) {    //checks that all following digits are numbers
        if (s[i] < '0' || s[i] > '9') {
            return 0;
        }
    }
    if (isUniqueSNum(s)) {   //checks that the student number is not a duplicate
        return 1;
    }
    return 0;
}

int isValidStudent(struct studentType *student) {
    if (!isValidSNum(student->sNum)) {   //checks that the student number is valid
        return 0;
    }
    if (student->mInitial < 'a' || student->mInitial > 'z') {    //checks that the middle initial is a lowercase letter
        return 0;
    }

    int i = 0;
    while (student->lName[i] && student->lName[i] != '\n') {      //checks that all characters in the last name are lowercase letters
        if (student->lName[i] < 'a' || student->lName[i] > 'z') {
            return 0;
        }
        ++i;
    }
    student->lName[i] = 0;      //fgets adds a newline to strings read into it unless they are the maximum length of the container.
                                //this line removes that newline

    i = 0;
    while (student->fName[i] && student->fName[i] != '\n') {      //checks that the first name is all lowercase letters
        if (student->fName[i] < 'a' || student->fName[i] > 'z') {
            return 0;
        }
        ++i;
    }
    student->fName[i] = 0;

    if (student->dob.year < MIN_YEAR || student->dob.year > MAX_YEAR) {  //checks that the student has a valid date of birth
        return 0;
    }
    if (student->dob.month < 1 || student->dob.month > 12) {
        return 0;
    }
    if (student->dob.day < 1 || student->dob.day > 30) {   //months with 31 days apparently do not exist
        return 0;
    }
    return 1;
}

void copyStudent(struct studentType *store, struct studentType *copy) {  //copies all fields except left and right from one student to another
    strcpy(store->sNum, copy->sNum);    //student number
    strcpy(store->lName, copy->lName);  //last name
    strcpy(store->fName, copy->fName);  //first name
    store->mInitial = copy->mInitial;   //middle initial
    store->dob.day = copy->dob.day;     //date of birth
    store->dob.month = copy->dob.month;
    store->dob.year = copy->dob.year;
    store->glasses = copy->glasses;     //whether or not the student has glasses
}

void addStudent(struct studentType *location, struct studentType *student) {     //adds a student to the binary search tree
                                                                                //external calls must pass root in as location
    int key = atoi(student->sNum + 1);

    if (key == ROOT_SNUM && !atoi(root->lName + 1)) {
        copyStudent(root, student);
        free(student);
    }
    if (atoi(location->sNum + 1) < key) {    //if the student must go to the right of the current location
        if (location->right == NULL) {       //if the next element to the right is empty, stores the student there
            location->right = student;
        }
        else {  //otherwise, continues on through the tree until it finds the correct placement for the student
            addStudent(location->right, student);
        }
    }
    else if (atoi(location->sNum + 1) > key) {   //if the student must go to the left of the current location
        if (location->left == NULL) {
            location->left = student;
        }
        else {
            addStudent(location->left, student);
        }
    }
}

struct studentType *findNextParent(struct studentType *student) {       //returns the parent of the next student

    if (student->left->left == NULL) {     //if the student to the left has no left child
            return student;
    }

    struct studentType *temp = findNextParent(student->left);

    return temp;
}

struct studentType *findParent(int key, struct studentType *location) {     //finds the parent of the key passed
    if (key == ROOT_SNUM) {      //returns 0 if there is no parent
        return NULL;
    }

    if (location->right && atoi(location->right->sNum + 1) == key) {    //returns the current location if it is the parent
        return location;
    }

    if (location->left && atoi(location->left->sNum + 1) == key) {      //returns the current location if it is the parent
        return location;
    }

    if (atoi(location->sNum + 1) < key) {       //if key is to the right of location
        if (location->right == NULL) {          //if key is not in the tree
            return NULL;
        }
        return findParent(key, location->right);
    }

    if (atoi(location->sNum + 1) > key) {       //key is to the left of location
        if (location->left == NULL) {
            return NULL;
        }
        return findParent(key, location->left);
    }
}

void deleteStudent(struct studentType *student, struct studentType *parent) {    //deletes an element from the binary search tree
    struct studentType *temp;
    struct studentType *nextParent;

    if (student == root) {      //was too lazy to set everything to zero if this was deleted, so I freed root and reallocated the memory with calloc
        struct studentType *tempLeft = student->left;
        struct studentType *tempRight = student->right;
        free(student);
        root = calloc(1, sizeof(struct studentType));
        root->left = tempLeft;
        root->right = tempRight;

        return;
    }

    if (student->left == NULL && student->right == NULL) {      //if student has no children
        if (parent->left == student && parent->right == NULL) {
            free(student);
            parent->left = NULL;
            return;
        }
        if (parent->right == student && parent->left == NULL) {
            free(student);
            parent->right = NULL;
            return;
        }
    }

    if (student->left == NULL && student->right) {       //if student has one child
        if (parent->left == student) {
            parent->left = student->right;
            free(student);
            return;
        }
        if (parent->right == student) {
            parent->right = student->right;
            free(student);
            return;
        }
    }
    if (student->right == NULL && student->left) {
        if (parent->left == student) {
            parent->left = student->left;
            free(student);
            return;
        }
        if (parent->right == student) {
            parent->right = student->left;
        }
    }

    if (student->left && student->right) {      //if student has two children
        if (student->right->left == NULL) {     //if student->right is the next student
            if (parent->left == student) {
                parent->left = student->right;
                free(student);
            }
            else {
                parent->right = student->right;
                free(student);
            }
            return;
        }

        else {
            nextParent = findNextParent(student);
            copyStudent(student, nextParent->left);
            if (nextParent->left->right == NULL) {     //if next student has no children
                free(nextParent->left);
            }
            else {
                temp = nextParent->left;
                nextParent->left = nextParent->left->right;
                free(temp);
            }
            return;
        }
    }
}

void getStudent() {
    fflush(stdin);

    struct studentType *temp = (struct studentType *) malloc(sizeof(struct studentType));

    printf("Student Number (lowercase 's'):\n");
    fgets(temp->sNum, SNUM_LEN + 1, stdin);
    fflush(stdin);

    printf("Last Name (all lowercase):\n");
    fgets(temp->lName, MAX_LNAME_LEN + 1, stdin);
    fflush(stdin);

    printf("First Name (all lowercase):\n");
    fgets(temp->fName, MAX_FNAME_LEN + 1, stdin);
    fflush(stdin);

    printf("Middle initial (lowercase):\n");
    scanf("%c", &temp->mInitial);
    fflush(stdin);

    printf("Date of birth:\n");
    printf("Year:\n");
    scanf("%i", &temp->dob.year);
    fflush(stdin);

    printf("Month:\n");
    scanf("%i", &temp->dob.month);
    fflush(stdin);

    printf("Day:\n");
    scanf("%i", &temp->dob.day);
    fflush(stdin);

    printf("Glasses ('Y' or 'N'):\n");
    scanf("%c", &temp->glasses);
    fflush(stdin);

    temp->left = NULL;
    temp->right = NULL;

    if (isValidStudent(temp)) {
        addStudent(root, temp);
        printf("Student added.\n\n");
    }
    else {
        free(temp);
        printf("Student could not be added.\n\n");
    }
}

void outputStudent(struct studentType *student) {
    printf("%s, %s, %s, %c, %i-%i-%i, %c\n", student->sNum, student->lName, student->fName, student->mInitial,
           student->dob.year, student->dob.month, student->dob.day, student->glasses);
}

void outputStudents(struct studentType *location){
    if (location->left != NULL) {
        outputStudents(location->left);
    }

    outputStudent(location);

    if (location->right != NULL) {
        outputStudents(location->right);
    }
}

void parseStudent(char *string) {
    struct studentType *student = calloc(1, sizeof(struct studentType));    //will hold the student being parsed from the file.
                                                                            //if the student is invalid, it will be freed.
    student->sNum[0] = 's';     //adds in the 's' that was skipped by lineStart in getStudentsFile();

    int length = 1;     //used to ensure that none of the fields in the student parsed too long
    while (*string != ',') {
        student->sNum[length] = *string;    //copies the string in the file into the student
        if (length == SNUM_LEN) {
            free(student);      //frees the student if anything is immediately invalid
            return;
        }
        ++length;
        ++string;
    }

    ++string;

    length = 0;
    while (*string != ',') {
        student->lName[length] = *string;
        if (length == MAX_LNAME_LEN) {
            free(student);
            return;
        }
        ++length;
        ++string;
    }

    ++string;

    length = 0;
    while (*string != ',') {
        student->fName[length] = *string;
        if (length == MAX_FNAME_LEN) {
            free(student);
            return;
        }
        ++length;
        ++string;
    }

    ++string;

    student->mInitial = *string;

    ++string;
    ++string;

    student->dob.day = atoi(string);
    while (*string != '/') {
        ++string;
    }
    ++string;

    student->dob.month = atoi(string);
    while (*string != '/') {
        ++string;
    }
    ++string;

    student->dob.year = atoi(string);
    while (*string != ',') {
        ++string;
    }
    ++string;

    student->glasses = *string;

    ++string;

    if (*string != '\n') {
        free(student);
        return;
    }

    if (!isValidStudent(student)) {
        free(student);
        return;
    }

    addStudent(root, student);
}

void getStudentsFile() {
    printf("\nType the name of the file you would like to read students from: \n");

    char fileName[100];
    gets(fileName);
    FILE *file = fopen(fileName, "r");

    char temp[100];
    char lineStart = fgetc(file);

    while (lineStart != EOF) {      //while there are still unread lines in the file
        if (lineStart != 's') {     //if the student number does not begin with an s
            continue;
        }

        fgets(temp, 100, file);
        parseStudent(temp);
        lineStart = fgetc(file);
    };

    fclose(file);
}

void outputStudentFile(struct studentType *student, FILE *file) {
    fprintf(file, "%s, %s, %s, %c, %i-%i-%i, %c\n", student->sNum, student->lName, student->fName, student->mInitial,
           student->dob.year, student->dob.month, student->dob.day, student->glasses);
}

void outputStudentsFile(struct studentType *location, FILE *file) {
    if (location->left != NULL) {
        outputStudentsFile(location->left, file);
    }

    outputStudentFile(location, file);

    if (location->right != NULL) {
        outputStudentsFile(location->right, file);
    }
}

int main() {

    fflush(stdin);
    root = calloc(1, sizeof(struct studentType));

    strcpy(root->sNum, ROOT_SNUM_STRING);       //to make sure the tree stays somewhat balanced, the root will always
                                                //point to the student number halfway between the min and max student numbers.
                                                //If the contents of root (besides sNum) are empty, it serves as a dummy
                                                //node and no student is considered to be stored there.
    struct studentType *temp;
    int userSelection = 0;
    while (userSelection != 6) {
        printf("(1) Add students from file\n"
               "(2) Add student manually\n"
               "(3) Delete student\n"
               "(4) Output to file (low to high by student number)\n"
               "(5) Print all students (low to high by student number)\n"
               "(6) Quit\n\n");
        scanf("%i", &userSelection);
        fflush(stdin);

        switch(userSelection) {
            case 1:
                getStudentsFile();
                break;
            case 2:
                getStudent();
                break;
            case 3:
                printf("Enter student number of student to be deleted (exclude the s):\n");
                scanf("%i", &userSelection);
                temp = binaryTreeSearch(userSelection, root);
                if (temp) {
                    deleteStudent(temp, findParent(userSelection, root));
                    printf("Student deleted.\n");
                }
                else {
                    printf("Invalid student.\n");
                }
                fflush(stdin);

                break;
            case 4:
                printf("\nType the name of the file you would like to write students to: \n");

                char fileName[100];
                gets(fileName);
                FILE *file = fopen(fileName, "w");

                outputStudentsFile(root, file);

                fclose(file);
                break;
            case 5:
                outputStudents(root);
                break;
            case 6:

                break;
            default:
                printf("Illegal input\n\n");
        }
    }
}
