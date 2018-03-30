/*
Social Network

Program to show how social networks are formed and how friends suggestions are basically made.

This project is licensed under the terms of the GNU GPLv3 license.
A copy of the lisence will be found in the root directory of the project as "LICENSE.txt"
*/
#include<ctype.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*
Structure used to represent the edges in a grpah. In this case an edge is a connection between two users. It contains ids of the two connected users (u1 and u2), pointer to the next node in the list of edges (next).
*/
struct edge {
	char u1[100], u2[100];
	struct edge *next;
} *conns;

/*
Sturture used to store friends suggestions. It contains the user id of the suggested friend (uid), number of mutual connection that the suggested friend has (mutual), pointer to the next suggestion in list (next).
*/
struct suggestion {
	int mutual;
	char uid[100];
	struct suggestion *next;
} *people;

/*
Structure used to represent a node in a linked list. It is used to store a list of vertices in a graph, list of friends of a person, etc. It contains the user id (uid) and pointer to the next node (next).
*/
struct node {
	char uid[100];
	struct node *next;
} *vertices, *friends;

//Function to add an edge/connection.
struct edge *addEdge(struct edge *list, char user1[], char user2[]);
//Function to add a node/person.
struct node *addNode(struct node *list, char user[]);
//Function to add a friend suggestion.
struct suggestion *addSugg(struct suggestion *list, int m, char user[]);
//Function to generate a list of adjacent nodes.
struct node *adjNodes(struct node *list, char user[]);
//Function to delete all edges of a person.
struct edge *delEdge1(struct edge *list, char user[]);
//Function to delete a specific edge.
struct edge *delEdge2(struct edge *list, char user1[], char user2[]);
//Function to delete a node.
struct node *delNode(struct node *list, char user[]);
//Function to check if an edge exists in a list. Returns 0/1 if edge does not/does exist.
int findEdge(struct edge *list, char user1[], char user2[]);
//Function to check if a node exists in a list. Returns 0/1 if node does not/does exist.
int findNode(struct node *list, char user[]);
//Function to check if a suggestion exists in a list. Returns 0/1 if suggestion does not/does exist.
int findSugg(struct suggestion *list, char user[]);
//Function to validate user id. Returns 0/1 if username is invalid/valid.
int validate(char a[]);
//Function to display all edges.
void dispEdge(struct edge *list);
//Function to display all node.
void dispNode(struct node *list);
//Function to display all suggestions.
void dispSugg(struct suggestion *list);
//Function to load pre-existing data.
void loadData();
//Function to save all data.
void saveData();
//Function to generare friend suggestions
void suggestFriends(char user[]);

struct edge *addEdge(struct edge *list, char user1[], char user2[]) {
	struct edge *head = (struct edge *)malloc(sizeof(struct edge));
	strcpy(head->u1, user1);
	strcpy(head->u2, user2);
	head->next = list;
	list = head;
	return list;
}

struct node *addNode(struct node *list, char user[]) {
	struct node *head = (struct node *)malloc(sizeof(struct node));
	strcpy(head->uid, user);
	head->next = list;
	list = head;
	return list;
}

struct suggestion *addSugg(struct suggestion *list, int m, char user[]) {
	struct suggestion *head = (struct suggestion *)malloc(sizeof(struct suggestion)), *prev = NULL, *temp = list;
	head->mutual = m;
	strcpy(head->uid, user);
	if((!list) || (list->mutual < m)  || ((list->mutual == m) && (strcmp(list->uid, user) > 0))) {
		head->next = list;
		list = head;
		return list;
	}
	while(temp) {
		if((temp->mutual < m) || ((temp->mutual == m) && (strcmp(temp->uid, user) > 0))) {
			prev->next = head;
			head->next = temp;
			return list;
		}
		prev = temp;
		temp = temp->next;
	}
	head->next = NULL;
	prev->next = head;
	return list;
}

struct node *adjNodes(struct node *list, char user[]){
	struct edge *temp = conns;
	struct node *temp2;
	while(temp) {
		if(strcmp(temp->u1, user) == 0) {
			temp2 = vertices;
			while(temp2) {
				if(strcmp(temp->u2, temp2->uid) == 0) {
					list = addNode(list, temp2->uid);
				}
				temp2 = temp2->next;
			}
		}
		if(strcmp(temp->u2, user) == 0) {
			temp2 = vertices;
			while(temp2) {
				if(strcmp(temp->u1, temp2->uid) == 0) {
					list = addNode(list, temp2->uid);
				}
				temp2 = temp2->next;
			}
		}
		temp = temp->next;
	}
	return list;
}

struct edge *delEdge1(struct edge *list, char user[]) {
	if(!list) {
		return list;
	}
	struct edge *prev = NULL, *temp = list;
	while(temp) {
		if(strcmp(temp->u1, user) == 0 || strcmp(temp->u2, user) == 0) {
			if(prev) {
				prev->next = temp->next;
			}
			else {
				list = temp->next;
				prev = NULL;
			}
		}
		else {
			prev = temp;
		}
		temp = temp->next;
	}
	return list;
}

struct edge *delEdge2(struct edge *list, char user1[], char user2[]) {
	if(!list) {
		return list;
	}
	struct edge *prev = NULL, *temp = list;
	while(temp) {
		if((strcmp(temp->u1, user1) == 0 && strcmp(temp->u2, user2) == 0) || (strcmp(temp->u1, user2) == 0 && strcmp(temp->u2, user1) == 0)) {
			if(prev) {
				prev->next = temp->next;
				return list;
			}
			list = temp->next;
			return list;
		}
		prev = temp;
		temp = temp->next;
	}
	return list;
}

struct node *delNode(struct node *list, char user[]) {
	if(!list) {
		return list;
	}
	struct node *prev = NULL, *temp = list;
	while(temp) {
		if(strcmp(temp->uid, user) == 0) {
			if(prev) {
				prev->next = temp->next;
				return list;
			}
			list = temp->next;
			return list;
		}
		prev = temp;
		temp = temp->next;
	}
	return list;
}

int findEdge(struct edge *list, char user1[], char user2[]) {
	if(!list) {
		return 0;
	}
	struct edge *temp = list;
	while(temp) {
		if((strcmp(temp->u1, user1) == 0 && strcmp(temp->u2, user2) == 0) || (strcmp(temp->u1, user2) == 0 && strcmp(temp->u2, user1) == 0)) {
			return 1;
		}
		temp = temp->next;
	}
	return 0;
}

int findNode(struct node *list, char user[]) {
	if(!list) {
		return 0;
	}
	struct node *temp = list;
	while(temp) {
		if(strcmp(temp->uid, user) == 0) {
			return 1;
		}
		temp = temp->next;
	}
	return 0;
}

int findSugg(struct suggestion *list, char user[]) {
	if(!list) {
		return 0;
	}
	struct suggestion *temp = list;
	while(temp) {
		if(strcmp(temp->uid, user) == 0) {
			return 1;
		}
		temp = temp->next;
	}
	return 0;
}

int validate(char a[]) {
	int l,i,f;
	f = 0;
	l = strlen(a);
	for(i = 0; i < l; ++i) {
		if(isalpha(a[i]) || isdigit(a[i]) || a[i]=='.') {
			f = 0;
		}
		else {
			f = 1;
			break;
		}
	}
	if(f == 0) {
      return 1;
	}
    if(f == 1) {
     return 0;
    }
}

void dispEdge(struct edge *list) {
	if(!list) {
		return;
	}
	struct edge *temp = list;
	printf("%s, %s", temp->u1, temp->u2);
	temp = temp->next;
	while(temp) {
		printf(" => %s, %s", temp->u1, temp->u2);
		temp = temp->next;
	}
	printf("\n");
}

void dispNode(struct node *list) {
	if(!list) {
		return;
	}
	struct node *temp = list;
	printf("%s", temp->uid);
	temp = temp->next;
	while(temp) {
		printf(" => %s", temp->uid);
		temp = temp->next;
	}
	printf("\n");
}

void dispSugg(struct suggestion *list) {
	if(!list) {
		return;
	}
	struct suggestion *temp = list;
	while(temp) {
		printf("%s\n%d mutual friends\n\n", temp->uid, temp->mutual);
		temp = temp->next;
	}
}

void loadData() {
	FILE *fptr;
	char n1[100], n2[100];
	fptr = fopen("conns.dat","a");
	fptr = fopen("users.dat","a");
	fptr = fopen("conns.dat","r");
	while(fscanf(fptr, "%s%s", n1, n2) != EOF) {
		conns = addEdge(conns, n1, n2);
	}
	fclose(fptr);
	fptr = fopen("users.dat", "r");
	while(fscanf(fptr, "%s", n1) != EOF) {
		vertices = addNode(vertices, n1);
	}
	fclose(fptr);
}

void saveData() {
	FILE *fptr;
	struct edge *temp1 = conns;
	struct node *temp2 = vertices;
	fptr = fopen("conns.dat","w");
	while(temp1) {
		fprintf(fptr, "%s\n%s\n", temp1->u1, temp1->u2);
		temp1 = temp1->next;
	}
	fclose(fptr);
	fptr = fopen("users.dat", "w");
	while(temp2) {
		fprintf(fptr, "%s\n", temp2->uid);
		temp2 = temp2->next;
	}
	fclose(fptr);
}

void suggestFriends(char user[]) {
	struct node *friends2, *friends3, *temp, *temp2, *temp3, *temp4;
	int count;
	friends = NULL;
	people = NULL;
	friends = adjNodes(friends, user);
	temp = friends;
	while(temp) {
		friends2 = NULL;
		friends2 = adjNodes(friends2, temp->uid);
		temp2 = friends2;
		while(temp2) {
			temp3 = friends;
			while(temp3) {
					if(strcmp(temp2->uid, temp3->uid) == 0 || strcmp(temp2->uid, user) == 0) {
						friends2 = delNode(friends2, temp2->uid);
					}
				temp3 = temp3->next;
			}
			temp2 = temp2->next;
		}
		temp2 = friends2;
		while(temp2) {
			friends3 = NULL;
			friends3 = adjNodes(friends3, temp2->uid);
			temp3 = friends3;
			count = 0;
			while(temp3) {
				temp4 = friends;
				while(temp4) {
					if(strcmp(temp3->uid, temp4->uid) == 0) {
						count++;
					}
					temp4 = temp4->next;
				}
				temp3 = temp3->next;
			}
			if(!findSugg(people, temp2->uid)) {
				people = addSugg(people, count, temp2->uid);
			}
			temp2 = temp2->next;
		}
		temp = temp->next;
	}
	if(people) {
		printf("\n\nFriend suggestions:-\n");
		dispSugg(people);
		return;
	}
	printf("\n\nNo suggestions.\n");
}

void main() {
	loadData();
	char n1[100], n2[100];
	int ch;
	char c;
	do {
		printf("Menu\n1. Add user\n2. Remove user\n3. Make Connection\n4. Remove Connection\n5. Suggest friends\n6. Display list of users\n7. Display list of connections\n8. Exit\nEnter your choice: ");
		scanf("%d", &ch);
		switch(ch) {
			case 1: {
				printf("Enter user ID: \n");
				c = getchar();
				scanf("%[^\n]", n1);
				ch = validate(n1);
				if(ch == 0) {
					printf("Invalid user ID. User ID can contain only alphabets, numbers and '.'.\n");
					break;
				}
				ch = findNode(vertices, n1);
				if(ch == 1) {
					printf("User already exists.\n");
					break;
				}
				vertices = addNode(vertices, n1);
				printf("User added.\n");
				break;
			}
			case 2: {
				printf("Enter user ID: \n");
				c = getchar();
				scanf("%[^\n]", n1);
				ch = validate(n1);
				if(ch == 0) {
					printf("Invalid user ID. User ID can contain only alphabets, numbers and '.'.\n");
					break;
				}
				ch = findNode(vertices, n1);
				if(ch == 0) {
					printf("User does not exist.\n");
					break;
				}
				vertices = delNode(vertices, n1);
				conns = delEdge1(conns, n1);
				printf("User removed.\n");
				break;
			}
			case 3: {
				printf("Enter 1st user ID: \n");
				c = getchar();
				scanf("%[^\n]", n1);
				ch = validate(n1);
				if(ch == 0) {
					printf("Invalid user ID. User ID can contain only alphabets, numbers and '.'.\n");
					break;
				}
				ch = findNode(vertices, n1);
				if(ch == 0) {
					printf("User does not exist.\n");
					break;
				}
				printf("Enter 2nd user ID: \n");
				c = getchar();
				scanf("%[^\n]", n2);
				ch = validate(n2);
				if(ch == 0) {
					printf("Invalid user ID. User ID can contain only alphabets, numbers and '.'.\n");
					break;
				}
				ch = findNode(vertices, n2);
				if(ch == 0) {
					printf("User does not exist.\n");
					break;
				}
				ch = findEdge(conns, n1, n2);
				if(ch == 1) {
					printf("Connection already exists.\n");
					break;
				}
				conns = addEdge(conns, n1, n2);
				printf("Connection made.\n");
				break;
			}
			case 4: {
				printf("Enter 1st user ID: \n");
				c = getchar();
				scanf("%[^\n]", n1);
				ch = validate(n1);
				if(ch == 0) {
					printf("Invalid user ID. User ID can contain only alphabets, numbers and '.'.\n");
					break;
				}
				ch = findNode(vertices, n1);
				if(ch == 0) {
					printf("User does not exist.\n");
					break;
				}
				printf("Enter 2nd user ID: \n");
				c = getchar();
				scanf("%[^\n]", n2);
				ch = validate(n2);
				if(ch == 0) {
					printf("Invalid user ID. User ID can contain only alphabets, numbers and '.'.\n");
					break;
				}
				ch = findNode(vertices, n2);
				if(ch == 0) {
					printf("User does not exist.\n");
					break;
				}
				ch = findEdge(conns, n1, n2);
				if(ch == 0) {
					printf("Connection does not exist.\n");
					break;
				}
				conns = delEdge2(conns, n1, n2);
				printf("Connection removed.\n");
				break;
			}
			case 5: {
				printf("Enter user ID: \n");
				c = getchar();
				scanf("%[^\n]", n1);
				ch = validate(n1);
				if(ch == 0) {
					printf("Invalid user ID. User ID can contain only alphabets, numbers and '.'.\n");
					break;
				}
				ch = findNode(vertices, n1);
				if(ch == 0) {
					printf("User does not exist.\n");
					break;
				}
				suggestFriends(n1);
				break;
			}
			case 6: {
				printf("List of users: \n");
				dispNode(vertices);
				break;
			}
			case 7: {
				printf("List of connections: \n");
				dispEdge(conns);
				break;
			}
			case 8: {
				break;
			}
			default: {
				printf("Invalid choice.\n");
			}
		}
		printf("\n\n");
	}while(ch != 8);
	saveData();
}
