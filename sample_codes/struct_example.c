
struct person{
	char *name;
	char *surname;
	int age;
	char *sex;
	char *email;
};

int main() 
{
	char *person1name = "Fatmanur";
	char *person1surname = "Karapýçak";
	int person1age = 26;
	char *person1sex = "Female";
	char *person1email = "ftmnrkrp@yildiz.edu.tr";
	
	char *person2name = "Ali";
	char *person2surname = "Sunal";
	int person2age = 32;
	char *person2sex = "Male";
	char *person2email = "alisunal@yildiz.edu.tr";
	
	//...
	
	struct person p1;
	p1.name = "Fatmanur";
	p1.surname = "Karapýçak";
	p1.age = 26;
	p1.sex = "Female";
	p1.email = "ftmnr...";
	
	printf("%d", p1.age);
	
	return 0; 
}
