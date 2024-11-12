#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

struct CitizenNode{
    int citizen_id;
    char name[100];
    char date_of_birth[50];
    char password[20];
    struct CitizenNode*left;
    struct CitizenNode*right;
};



// insertion of citizen
struct CitizenNode*insertCitizen(struct CitizenNode*root, int citizen_id, char name[], char date_of_birth[]){
    if(root==NULL){
        struct CitizenNode*new_node = malloc(sizeof(struct CitizenNode));
        new_node->citizen_id = citizen_id;
        strcpy(new_node->name, name);
        strcpy(new_node->date_of_birth, date_of_birth);
        int i = 0;
        int j = 0;
        while (i<10){
            if(date_of_birth[i]!='-'){
                new_node->password[j] = date_of_birth[i];
                j++;
            }
            i++;
        }
        new_node->password[j] ='\0';
        new_node->left = new_node->right = NULL;
        return new_node;
    }
    if(citizen_id < root->citizen_id){
        root->left = insertCitizen(root->left, citizen_id, name, date_of_birth);
    }
    else if(citizen_id > root->citizen_id){
        root->right = insertCitizen(root->right, citizen_id, name, date_of_birth);
    }
    else{
        printf("\n Try Again \n");
    }
    return root;
}



struct CitizenNode*searchCitizen(struct CitizenNode*root, int target_id){
    if(root==NULL || root->citizen_id==target_id){
        return root;
    }
    if(root->citizen_id > target_id){
        return searchCitizen(root->left, target_id);
    }
    else{
        return searchCitizen(root->right, target_id);
    }
}

// deletion


struct CitizenNode* findMin(struct CitizenNode* node) {
    if (node == NULL || node->left == NULL) {
        return node;
    }
    return findMin(node->left);
}


struct CitizenNode* deleteCitizen(struct CitizenNode* root, int target_id) {
    if (root == NULL) {
        printf(" \n The Database doesnot contain any Citizens \n");
        return root;
    }

    if (target_id < root->citizen_id) {
        root->left = deleteCitizen(root->left, target_id);
    } 
    else if (target_id > root->citizen_id) {
        root->right = deleteCitizen(root->right, target_id);
    } 
    else {

        if (root->left == NULL && root->right == NULL) {
            free(root);
            root = NULL;
        } 
        else if (root->left == NULL) {
            struct CitizenNode* temp = root->right;
            free(root);
            root = temp;
        } 
        else if (root->right == NULL) {
            struct CitizenNode* temp = root->left;
            free(root);
            root = temp;
        } 
        else {
            struct CitizenNode* temp = findMin(root->right);
            root->citizen_id = temp->citizen_id;
            strcpy(root->name, temp->name);
            strcpy(root->date_of_birth, temp->date_of_birth);
            strcpy(root->password, temp->password);
            root->right = deleteCitizen(root->right, temp->citizen_id);
        }
    }

    return root;
}

// checking password

int check_password(int citizen_id, struct CitizenNode*root, char passwd[]){
    root = searchCitizen(root, citizen_id);
    if(root==NULL){
        printf("\n Citizen ID not found. \n");
        return -1;
    }
    return strcmp(passwd, root->password) == 0 ? 0:-1;


}


int main(){
    int command = -1;
    int id = 0;
    char passwd[20];
    int key = -1;
    struct CitizenNode*root = NULL;
    printf("\nWelcome to National Citizen Database Model \n");
    printf("designed by : KARRI TIRUMALA VENKATESH\n");
    
    while(command !=0){
        printf("\n0. Exit \n");
        printf("1. Add new Citizen \n");
        printf("2. Remove a Citizen \n");

        printf("\nEnter the Command number to be performed : ");
        scanf("%d", &command);

        switch (command)
        {
        case 1:
            srand(time(NULL));
            int num = (rand()%9000)+1000;
            printf("\nEnter Citizen Name : ");
            char name[30];
            scanf("%s", name);
            printf("Enter Citizen Date of Birth as DD-MM-YYYY : ");
            char dob[30];
            scanf("%s", dob);
            root = insertCitizen(root, num, name, dob);
            
            printf("\nCongrats Citizen, you are successfully added to the National Citizen Database\n");
            printf("%d is your new Citizen ID and Password is your Date of Birth as DDMMYYYY\n", num);
            break;

        case 2:
            printf("\nEnter Citizen ID : ");
            id = 0;
            scanf("%d", &id);
            printf("Enter Citizen Password : ");
            scanf("%s", passwd);
            key = check_password(id, root, passwd);
            if(key==0){
                root=deleteCitizen( root, id);
                printf("\nCitizen successfully deleted.\n");
            }
            else{
                printf("\nIncorrect Citizen ID or Password\n");
            }
            break;

        default:
            break;
        }
    }
    printf("\nThank You\n\n");
   return 0;

}