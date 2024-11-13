#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

struct LinkedListNode {
    char data_topic[50];
    char data[100];
    struct LinkedListNode *next;
};

struct CitizenNode{
    int citizen_id;
    char name[100];
    char date_of_birth[50];
    char password[20];
    struct LinkedListNode*linked_list_head;
    struct CitizenNode*left;
    struct CitizenNode*right;
};

// adding detail to linked list
void addToList(struct CitizenNode* citizen, char new_topic[], char new_data[]) {
    struct LinkedListNode*new_node = malloc(sizeof(struct LinkedListNode));
    strcpy(new_node->data_topic, new_topic);
    strcpy(new_node->data, new_data);
    new_node->next = NULL;

    if (citizen->linked_list_head == NULL) {
        citizen->linked_list_head = new_node;
    } 
    else {
        struct LinkedListNode *current = citizen->linked_list_head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}

// updating a detail in a linked list
int updateList(struct CitizenNode* citizen, char data_topic[], char new_data[]) {
    if (citizen->linked_list_head == NULL) {
        printf("\n No additional details of the Citizen are available \n");
        return 0;
    }
    struct LinkedListNode *current = citizen->linked_list_head;
    while (current != NULL) {
        if (strcmp(current->data_topic, data_topic) == 0) {
            strcpy(current->data, new_data);
            return 0;
        }
        current = current->next;
    }
    printf("\n Require details of the Citizen are not available \n");
    return 0;
}

// deleting a detail in a linked list
int deleteFromList(struct CitizenNode* citizen, char data_topic[]) {
    if (citizen->linked_list_head == NULL) {
        printf("\n No additional details of the Citizen are available \n");
        return 0;
    }

    if (strcmp(citizen->linked_list_head->data_topic, data_topic) == 0) {
        struct LinkedListNode *temp = citizen->linked_list_head;
        citizen->linked_list_head = citizen->linked_list_head->next;
        free(temp);
        return 0;
    }
    struct LinkedListNode *current = citizen->linked_list_head;
    while (current != NULL && current->next != NULL) {
        if (strcmp(current->next->data_topic, data_topic) == 0) {
            struct LinkedListNode *temp = current->next;
            current->next = current->next->next;
            free(temp);
            return 0;
        }
        current = current->next;
    }
    printf("\n Require details of the Citizen are not available \n");
    return 0;
}

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
        new_node->linked_list_head = NULL;
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
            root->linked_list_head=temp->linked_list_head;
            root->right = deleteCitizen(root->right, temp->citizen_id);
        }
    }

    return root;
}

// updation

int update_citizen_name(int citizen_id, char new_name[], struct CitizenNode* root){
    root = searchCitizen(root, citizen_id);
    if(root==NULL){
        printf("\n Citizen ID not found. \n");
        return -1;
    }
    strcpy(root->name, new_name);
    return 0;
}

// writing into a text file

int writeLinkedListToFileById(int citizen_id, struct CitizenNode* root) {
    if (root == NULL) {
        return -1;
    }
    struct CitizenNode* citizen = searchCitizen(root, citizen_id);
    if (citizen == NULL) {
        return -2;
    }
    char str[15];
    sprintf(str, "%d.txt", citizen_id);
    FILE *list_file = fopen(str, "w");
    if (list_file == NULL) {
        return -3;
    }
    struct LinkedListNode *current = citizen->linked_list_head;
    while (current != NULL) {
        fprintf(list_file, "%s : %s\n", current->data_topic, current->data);
        current = current->next;
    }
    fclose(list_file);
    return 0;
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
        printf("3. Update Citizen Name \n");
        printf("4. Search for Citizen \n");
        printf("5. Add an Additional Detail to Citizen \n");
        printf("6. Update an Additional Detail of a Citizen \n");
        printf("7. Delete an Additional Detail of a Citizen \n");

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
                printf("\nCitizen successfully deleted from Database.\n");
            }
            else{
                printf("\nIncorrect Citizen ID or Password\n");
            }
            break;


        case 3:
            printf("\nEnter Citizen ID : ");
            id = 0;
            scanf("%d", &id);
            printf("Enter Citizen Password : ");
            scanf("%s", passwd);
            key = check_password(id, root, passwd);
            if(key==0){
                printf("Enter Citizen NEW Name : ");
                char new_name[30];
                scanf("%s", new_name);
                update_citizen_name(id,  new_name, root);
                printf("\nCitizen Name successfully updated.\n");
            }
            else{
                printf("\nIncorrect Citizen ID or Password\n");
            }
            break;

        case 4:
            printf("\nEnter Citizen ID : ");
            id = 0;
            scanf("%d", &id);
            printf("Enter Citizen Password : ");
            scanf("%s", passwd);
            key = check_password(id, root, passwd);
            if(key==0){
                struct CitizenNode*node = searchCitizen(root, id);
                printf("Citizen Name : %s\n", node->name);
                printf("Citizen ID : %d\n", node->citizen_id);
                printf("Date of Birth : %s\n", node->date_of_birth);
                struct LinkedListNode *current = node->linked_list_head;
                while (current != NULL) {
                    printf("%s : %s\n", current->data_topic, current->data);
                    current = current->next;
                }   
            }
            else{
                printf("\nIncorrect Citizen ID or Password\n");
            }
            break; 

        case 5:
            printf("\nEnter Citizen ID : ");
            id = 0;
            scanf("%d", &id);
            printf("Enter Citizen Password : ");
            scanf("%s", passwd);
            key = check_password(id, root, passwd);
            if(key==0){
                printf("Enter the additional detail of the citizen to be added : ");
                char new_detail[30];
                scanf("%s", new_detail);
                printf("Enter the Data of the additional detail : ");
                char new_data[30];
                scanf("%s", new_data);
                struct CitizenNode*citizen = searchCitizen(root, id);
                addToList(citizen, new_detail, new_data);
                writeLinkedListToFileById( id, root);
            }
            else{
                printf("\nIncorrect Citizen ID or Password\n");
            }
            break;

        case 6:
            printf("\nEnter Citizen ID : ");
            id = 0;
            scanf("%d", &id);
            printf("Enter Citizen Password : ");
            scanf("%s", passwd);
            key = check_password(id, root, passwd);
            if(key==0){
                printf("Enter the additional detail of the citizen to be Updated : ");
                char detail[30];
                scanf("%s", detail);
                printf("Enter the New Data of the additional detail : ");
                char data[30];
                scanf("%s", data);
                struct CitizenNode*citizen = searchCitizen(root, id);
                updateList(citizen, detail, data);
                writeLinkedListToFileById( id, root);
            }
            else{
                printf("\nIncorrect Citizen ID or Password\n");
            }
            break;


        case 7:
            printf("\nEnter Citizen ID : ");
            id = 0;
            scanf("%d", &id);
            printf("Enter Citizen Password : ");
            scanf("%s", passwd);
            key = check_password(id, root, passwd);
            if(key==0){
                printf("Enter the additional detail of the citizen to be deleted : ");
                char detail[30];
                scanf("%s", detail);
                struct CitizenNode*citizen = searchCitizen(root, id);
                deleteFromList( citizen, detail);
                writeLinkedListToFileById( id, root);
            }
            else{
                printf("\nIncorrect Citizen ID or Password\n");
            }
            break;
        
        default:
            break;
        }
    }
    printf("\n Thank You \n");
   return 0;

}