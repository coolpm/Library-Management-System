#include<stdio.h>
#include<conio.h>
#include<time.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
const int monthDays[12] = {31, 28, 31, 30, 31, 30,
                           31, 31, 30, 31, 30, 31};
struct date_str
{
  int date,month,year;
};
struct book
{
    int book_id;
    char book_name[25];
    char dept[25];
    int status;
    struct book *link_book;

};
struct book *start_book=NULL; //for pointing to first node
struct book *cur_book=NULL; //pointer for traversal
struct book *temp_book=NULL; // pointer to allocate temporary dynamic memory to store data of books in library(total)
// structure for book complete
struct book_issued
{
    int book_id;
    char book_name[25];
    int stu_id;
    // for storing issued date and return date=>
    struct date_str issued_date,return_date;
    struct book_issued *link_issued;
};
struct book_issued *start_issued=NULL; //for pointing to first node.
struct book_issued *cur_issued=NULL; //pointer for traversal.
struct book_issued *temp_issued=NULL; // pointer to allocate temporary dynamic memory to store data of issued books in library
// structure for books issued complete
struct reg_students
{
    int stu_id;
    char stu_name[25];
    int fine;
    int books_issued;
    struct reg_students *link_stu;
};
struct reg_students *start_stu=NULL; //pointer that will point to first node of link list of details of registered students
struct reg_students *cur_stu=NULL; //pointer for traversal and linking in link list of details of registered students
struct reg_students *temp_stu=NULL; // pointer to allocate temporary dynamic memory to store data of students
// structure for students complete
struct book_dept
{
    char dept_name[25];
    int total;//finds total number of books in that particular department
    int available;// finds available books in library based on department
};
// structure making complete
/** function declarations of time and date **/
struct date_str curdate();
int countLeapYears(struct date_str d);
int offsetDays(int date, int month, int year);
int getDifference(struct date_str dt1,struct date_str dt2);
int diff(struct date_str return_date);
int isLeap(int year);
void revoffsetDays(int offset, int year, int *date, int *mon);
struct date_str returndate();
struct date_str addDays(int date, int month, int year, int num);
/** function declarations of getting and putting data in files**/
void getdata_book();
void getdata_issued();
void getdata_stu();
void putdata_book();
void putdata_issued();
void putdata_stu();
/** function to copy details of students into link lists **/
void getdata()
{
    getdata_stu();
    getdata_book();
    getdata_issued();
}
/** function to save data in files **/
void putdata()
{
    remove("book.txt");
    remove("book_issued.txt");
    remove("reg_student.txt");
    putdata_book();
    putdata_issued();
    putdata_stu();
}
/** fuction to getdata of registered student **/
void getdata_stu()
{
    FILE *fp_stu;
    fp_stu=fopen("reg_student.txt","r");
    if(fp_stu!=NULL)
    {
        while(!feof(fp_stu))
        {
            temp_stu=(struct reg_students *)malloc(sizeof(struct reg_students));
            temp_stu->link_stu=NULL;
            fscanf(fp_stu,"%d%d%d%s",&temp_stu->stu_id,&temp_stu->fine,&temp_stu->books_issued,temp_stu->stu_name);
            fscanf(fp_stu,"\n");
            if(start_stu==NULL)
            {
                start_stu=temp_stu;
                cur_stu=start_stu;
            }
            else
            {
                cur_stu->link_stu=temp_stu;
                cur_stu=cur_stu->link_stu;
            }
        }
    }
    fclose(fp_stu);
}
/** function to copy details of total books installed in library into link lists **/
void getdata_book()
{
    FILE *fp_book;
    fp_book=fopen("book.txt","r");
    if(fp_book!=NULL)
    {
        while(!feof(fp_book))
        {
            temp_book=(struct book *)malloc(sizeof(struct book));
            temp_book->link_book=NULL;
            fscanf(fp_book,"%s%d%d%s",temp_book->dept,&temp_book->book_id,&temp_book->status,temp_book->book_name);
             fscanf(fp_book,"\n");
            if(start_book==NULL)
            {
                start_book=temp_book;
                cur_book=temp_book;
            }
            else
            {
                cur_book->link_book=temp_book;
                cur_book=cur_book->link_book;
            }
        }
    }
    fclose(fp_book);
}
/** function to copy details of issued books into link lists **/
void getdata_issued()
{
    FILE *fp_issued;
    fp_issued=fopen("book_issued.txt","r");
    if(fp_issued!=NULL)
    {
        while(!feof(fp_issued))
        {
            temp_issued=(struct book_issued *)malloc(sizeof(struct book_issued));
            temp_issued->link_issued=NULL;
            fscanf(fp_issued,"%d%d%d/%d/%d",&temp_issued->book_id,&temp_issued->stu_id,&temp_issued->issued_date.date,&temp_issued->issued_date.month,&temp_issued->issued_date.year);
            fscanf(fp_issued,"%d/%d/%d%s",&temp_issued->return_date.date,&temp_issued->return_date.month,&temp_issued->return_date.year,temp_issued->book_name);
            fscanf(fp_issued,"\n");
            if(start_issued==NULL)
            {
                start_issued=temp_issued;
                cur_issued=temp_issued;
            }
            else
            {
                cur_issued->link_issued=temp_issued;
                cur_issued=cur_issued->link_issued;
            }
        }
    }
    fclose(fp_issued);
}
/** function for copying data from link lists to book file **/
void putdata_book()
{
    if(start_book!=NULL)
    {
        FILE *fp_book;
        fp_book=fopen("book.txt","w");
        cur_book=start_book;
        while(cur_book!=NULL)
        {
            fprintf(fp_book,"%s\t%d\t%d\t%s\n",cur_book->dept,cur_book->book_id,cur_book->status,cur_book->book_name);
            temp_book=cur_book;
            cur_book=cur_book->link_book;
            free(temp_book);
        }
        fclose(fp_book);
    }
}
/** function for copying data from link lists to reg_student file **/
void putdata_stu()
{
    if(start_stu!=NULL)
    {
        FILE *fp_stu;
        fp_stu=fopen("reg_student.txt","w");
        cur_stu=start_stu;
        while(cur_stu!=NULL)
        {
            fprintf(fp_stu,"%d\t%d\t%d\t%s\n",cur_stu->stu_id,cur_stu->fine,cur_stu->books_issued,cur_stu->stu_name);
            temp_stu=cur_stu;
            cur_stu=cur_stu->link_stu;
            free(temp_stu);
        }
        fclose(fp_stu);
    }
}
/** function for copying data from link lists to book_issued file **/
void putdata_issued()
{
    if(start_issued!=NULL)
    {
        FILE *fp_issued;
        fp_issued=fopen("book_issued.txt","w");
        cur_issued=start_issued;
        while(cur_issued!=NULL)
        {
            fprintf(fp_issued,"%d\t%d\t%d/%d/%d\t",cur_issued->book_id,cur_issued->stu_id,cur_issued->issued_date.date,cur_issued->issued_date.month,cur_issued->issued_date.year);
            fprintf(fp_issued,"%d/%d/%d\t%s\n",cur_issued->return_date.date,cur_issued->return_date.month,cur_issued->return_date.year,cur_issued->book_name);
            temp_issued=cur_issued;
            cur_issued=cur_issued->link_issued;
            free(temp_issued);
        }
        fclose(fp_issued);
    }
}
/** function to add new student in system **/
void add_stu()
{
    int status=1;  // status 1 represents id is unique
    cur_stu=start_stu;
    temp_stu=(struct reg_students *)malloc(sizeof(struct reg_students));
    temp_stu->link_stu=NULL;
    printf("\n enter enroll id of new student registered with library :  ");
    scanf("%d",&temp_stu->stu_id);
    while(cur_stu!=NULL)
    {
        if((cur_stu->stu_id)==temp_stu->stu_id)    // code to check uniqueness of student id and getting the new unique id.
        {
            free(temp_stu);
            printf("provided id already exists so cannot add new student.");
            status=0; // status 0 represents id already exists
            break;
        }
        cur_stu=cur_stu->link_stu;
    }
    if(status==1)
    {
    printf("enter student name with id %d :  ",temp_stu->stu_id);
    scanf("\n%[^\n]",temp_stu->stu_name);
    temp_stu->fine=0;
    temp_stu->books_issued=0;
    // for cur_stu to point to last node
    if(start_stu==NULL)
    {
        start_stu=temp_stu;
        cur_stu=start_stu;
    }
    else
    {
        cur_stu=start_stu;
        while(cur_stu->link_stu!=NULL) // loop for pointing to last node
           cur_stu=cur_stu->link_stu;
        cur_stu->link_stu=temp_stu;
        cur_stu=cur_stu->link_stu;
    }
    printf("\n adding of new student to the system is successful \n");
    }
}
/** function to delete record of a student **/
void delete_stu()
{
    temp_stu=NULL;
    int status=0;
    cur_stu=start_stu;
    int temp_id;//to get id of the student to be deleted.
    printf("enter unique id of student to delete student details from the system :  ");
    scanf("%d",&temp_id);
    while(cur_stu!=NULL)  // check whether student registered or not.
    {
        if(cur_stu->stu_id==temp_id)
        {
            status=1; // status 1 means student with provided id is found in data
            break;
        }
        temp_stu=cur_stu;
        cur_stu=cur_stu->link_stu;
    }
    if(status==0)
        printf("\n\nwrong input of student enrollment id\n student with provided id does not exist. \n");
    else //cur_stu now pointing to student to be removed from system.
    {
        if(cur_stu->books_issued!=0)
        {
            printf("\n\n cannot remove the student as %d books are already issued to the student\n first return those books \n",cur_stu->books_issued);
        }
        else
        {
            int status_fine=1;
            while(status_fine)
            {
                if(cur_stu->fine!=0)
                {
                    printf("\n\n fine pending rupees %d\n",cur_stu->fine);
                    int choice;
                    printf("\npress 1.if student is paying fine now and 0. if not paying\n :  ");
                    scanf("%d",&choice);
                    if(choice==0)
                    {
                        printf("\n cannot remove student due to fine \n");
                        status_fine=0;
                    }
                    else
                    {
                        printf("\n\n take fine from student as now fine in his account will be null \n");
                        printf("fine should be completely paid \n");
                        cur_stu->fine=0;
                        printf("fine updated to zero(0)");
                    }
                }
                else //means fine is 0(zero)
                {
                    if(temp_stu==NULL)
                    {
                        temp_stu=start_stu;
                        start_stu=cur_stu->link_stu;
                        free(temp_stu);
                    }
                    else
                    {
                        temp_stu->link_stu=cur_stu->link_stu;
                        temp_stu=cur_stu;
                        free(temp_stu);
                    }
                    printf("\n successfully deleted the student from the system. \n");
                    status_fine=0;
                }
            }
        }
    }
}
/** function to add new book in library **/
void add_book()
{
    int check=1;
    int choice;
    int status=1;
    cur_book=start_book; // for traversal
    temp_book=(struct book *)malloc(sizeof(struct book));
    temp_book->link_book=NULL;
    printf("enter new book id alloted to book to be add in library :  ");
    scanf("%d",&temp_book->book_id);
    while(cur_book!=NULL)
    {
        if((cur_book->book_id)==temp_book->book_id)    // code to check uniqueness of student id and getting the new unique id.
        {
            free(temp_book);
            printf("\n provided id already exists so cannot add new book. \n");
            status=0; // status 0 represents id already exists
            break;
        }
        cur_book=cur_book->link_book;
    }
    if(status==1)// means id available as unique
    {
        printf("\nenter name of the book :  ");
        scanf("\n%[^\n]",temp_book->book_name);
        printf("\n enter choice in integer for book department among following :  ");
        while(check)
        {
        printf("\n 1.computer \n 2.electrical \n 3.mathematics \n 4.english \n 5.physics \n 6.general \n 7.comic :  \n");
        scanf("%d",&choice);
        switch(choice) // for copying the perfect department name to book records
          {
        case 1:
            strcpy(temp_book->dept,"computer");
            check=0;
            break;
        case 2:
            strcpy(temp_book->dept,"electrical");
            check=0;
            break;
        case 3:
            strcpy(temp_book->dept,"mathematics");
            check=0;
            break;
        case 4:
            strcpy(temp_book->dept,"english");
            check=0;
            break;
        case 5:
            strcpy(temp_book->dept,"physics");
            check=0;
            break;
        case 6:
            strcpy(temp_book->dept,"general");
            check=0;
            break;
        case 7:
            strcpy(temp_book->dept,"comic");
            check=0;
            break;
        default:
            printf("\nwrong input\n");
            break;
          }
        }
        temp_book->status=1;
        if(start_book==NULL)
        {
            start_book=temp_book;
            cur_book=start_book;
        }
        else
        {
            // cur_book will now point to last node
            cur_book=start_book;
            while(cur_book->link_book!=NULL)
                cur_book=cur_book->link_book;
                // adding new node
            cur_book->link_book=temp_book;
            cur_book=cur_book->link_book;
        }
        printf("\n %d %s %s %d \n",cur_book->book_id,cur_book->book_name,cur_book->dept,cur_book->status);
        printf("\n adding new book in system is successfully done.\n");
        // updating file dept as new book is added to particular department
        int status_dept=1; // for checking initially department is in the file or not.
        struct book_dept dept;
        FILE *fp_dept;
        fp_dept=fopen("book_dept.txt","r");
        FILE *temp_dept;
        temp_dept=fopen("temp_dept.txt","w");
        if(fp_dept==NULL) //for first initial book
            fprintf(temp_dept,"%s\t%d\t%d\n",temp_book->dept,1,1);
        else
        {
            while(!feof(fp_dept))
                  {
                      fscanf(fp_dept,"%s%d%d",dept.dept_name,&dept.total,&dept.available);
                      fscanf(fp_dept,"\n");
                      if(strcmp(dept.dept_name,temp_book->dept)==0) //means both strings are same =>department matched with newly added book's department
                      {
                          status_dept=0;
                          dept.total++;
                          dept.available++;
                      }
                      fprintf(temp_dept,"%s\t%d\t%d\n",dept.dept_name,dept.total,dept.available);
                  }
                  if(status_dept==1) // department wise first initial book
                      fprintf(temp_dept,"%s\t%d\t%d\n",temp_book->dept,1,1);
        }
        fclose(fp_dept);
        fclose(temp_dept);
        remove("book_dept.txt");
        rename("temp_dept.txt","book_dept.txt");
    }
}
/** function to remove am book from library system **/
void delete_book()
{
    int status=0; // for checking if book id exists or not
    int temp_id; // to get id of the book to be deleted
    temp_book=NULL;
    cur_book=start_book;
    printf("enter the unique book id of the book to be deleted :  ");
    scanf("%d",&temp_id);
    while(cur_book!=NULL)
    {
        if(cur_book->book_id==temp_id)
        {
            status=1; // means that book exists
            break;
        }
        temp_book=cur_book;
        cur_book=cur_book->link_book;
    }
    if(status==0)
        printf("\n\nwrong input of book id\n book with provided id does not exist\n");
    else if(cur_book->status==0)
      printf("\n book is issued so cannot be deleted from system \n");
    else
    {
        if(temp_book==NULL)
        {
            temp_book=start_book;
            start_book=cur_book->link_book;
        }
        else
        {
            temp_book->link_book=cur_book->link_book;
            temp_book=cur_book;
        }
         printf("successfully deleted book from system.");
         // now updating file book_dept.txt
        struct book_dept dept;
        FILE *fp_dept;
        fp_dept=fopen("book_dept.txt","r");
        FILE *temp_dept;
        temp_dept=fopen("temp_dept.txt","w");
        while(!feof(fp_dept))
        {
            fscanf(fp_dept,"%s%d%d",dept.dept_name,&dept.total,&dept.available);
            fscanf(fp_dept,"\n");
            if(strcmp(dept.dept_name,temp_book->dept)==0)
            {
                dept.total--;
                dept.available--;
            }
            fprintf(temp_dept,"%s\t%d\t%d\n",dept.dept_name,dept.total,dept.available);
        }
        free(temp_book);
        fclose(fp_dept);
        fclose(temp_dept);
        remove("book_dept.txt");
        rename("temp_dept.txt","book_dept.txt");
    }
}
/** function for issuing a book  by a registered student **/
void issue_book()
{
    temp_issued=(struct book_issued *)malloc(sizeof(struct book_issued));
    temp_issued->link_issued=NULL;
    cur_issued=start_issued;
    int status_stu=0; // for checking a student is registered or not
    printf("\n enter student enrollment id of the student to issue book :  ");
    scanf("%d",&temp_issued->stu_id);
    cur_stu=start_stu;
    while(cur_stu!=NULL)
    {
        if(temp_issued->stu_id==cur_stu->stu_id)
        {
            status_stu=1;
            break;
        }
        cur_stu=cur_stu->link_stu;
    }
    if(status_stu==0)// means student is not registered.
            {
                free(temp_issued);
                printf("\n cannot issue book as student is not registered \n");
            }
    else // student is registered.
    {
        //cur_stu pointer now points to node having details  of student.
        if(cur_stu->books_issued==5)
            {
                printf("\n cannot issue more books as 5 books are already issued by the student \n");
                free(temp_issued);
            }
        else // book can be issued by the student.
        {
            int status_fine=1;
            while(status_fine)
            {
                if(cur_stu->fine!=0)
                {
                    int choice;
                    printf("\n\n fine pending rupees %d\n",cur_stu->fine);
                    printf("\npress 1.if student is paying fine now and 0. if not paying\n :  ");
                    scanf("%d",&choice);
                    if(choice==0)
                    {
                        printf("\n cannot remove student due to fine \n");
                        status_fine=0;
                    }
                    else
                    {
                        printf("\n\n take fine from student as now fine in his account will be null");
                        printf("fine should be completely paid");
                        cur_stu->fine=0;
                        printf("fine updated to zero(0)");
                    }
                }
                else //  fine is zero.
                {
                    status_fine=0;
                    int status_book=0;
                    temp_book=(struct book *)malloc(sizeof(struct book));
                    temp_book->link_book=NULL;
                    printf("\n enter id of the book to issue by student with student id = %d :  ",cur_stu->stu_id);
                    scanf("%d",&temp_book->book_id);
                    cur_book=start_book;
                    while(cur_book!=NULL)
                    {
                        if(cur_book->book_id==temp_book->book_id)
                        {
                            if(cur_book->status==1)
                            {
                            status_book=1;
                            break;
                            }
                            else
                            {
                                printf("\n book is already issued. \n");
                                break;
                            }
                        }
                        cur_book=cur_book->link_book;
                    }
                    if(status_book==0)
                    {
                        printf("\n book not available \n");
                        free(temp_book);
                    }
                    else // book with provided book id is found (status_book=1)
                    {
                        cur_stu->books_issued++;
                        cur_book->status=0;
                        temp_issued->book_id=temp_book->book_id;
                        strcpy(temp_issued->book_name,cur_book->book_name);
                        temp_issued->issued_date=curdate();
                        temp_issued->return_date=returndate();
                        if(start_issued==NULL)
                        {
                            start_issued=temp_issued;
                            cur_issued=start_issued;
                        }
                        else
                        {
                            cur_issued=start_issued;
                            while(cur_issued->link_issued!=NULL)
                                cur_issued=cur_issued->link_issued;
                            cur_issued->link_issued=temp_issued;
                            cur_issued=cur_issued->link_issued;
                            printf("\n book successfully issued \n");
                        }
                        // updating file dept_book.
                        struct book_dept dept;
                        FILE *fp_dept;
                        fp_dept=fopen("book_dept.txt","r");
                        FILE *temp_dept;
                        temp_dept=fopen("temp_dept.txt","w");
                        if(fp_dept!=NULL)
                        while(!feof(fp_dept))
                        {
                            fscanf(fp_dept,"%s%d%d",dept.dept_name,&dept.total,&dept.available);
                            fscanf(fp_dept,"\n");
                            if(strcmp(dept.dept_name,cur_book->dept)==0) // department matched
                                dept.available--;
                            fprintf(temp_dept,"%s\t%d\t%d\n",dept.dept_name,dept.total,dept.available);
                        }
                        fclose(fp_dept);
                        fclose(temp_dept);
                        remove("book_dept.txt");
                        rename("temp_dept.txt","book_dept.txt");
                    }
                }
            }
        }
    }
}
/** function to return a book **/
void return_book()
{
    temp_issued=NULL;
    int status=0;
    cur_issued=start_issued;
    int book_id;
    printf("\n enter book id  to return :  ");
    scanf("%d",&book_id);
    while(cur_issued!=NULL)
    {
        if(cur_issued->book_id==book_id)
        {
            status=1; // means book is one of the issued books of the system.
            break;
        }
        temp_issued=cur_issued;
        cur_issued=cur_issued->link_issued;
    }
    if(status==0)
        printf("\n book is not related to issued books  \n");
    else // book is among issued books of library
    {
       if(temp_issued==NULL)
       {
           temp_issued=cur_issued;
           start_issued=cur_issued->link_issued;
       }
       else
        {
            temp_issued->link_issued=cur_issued->link_issued;
            temp_issued=cur_issued;
        }
        cur_book=start_book;
        // making status 1 of book in library.
        while(cur_book!=NULL)
        {
            if(cur_book->book_id==temp_issued->book_id)
            {
                cur_book->status=1;
                break;
            }
            cur_book=cur_book->link_book;
        }
        cur_stu=start_stu;
        while(cur_stu!=NULL)
        {
            if(cur_stu->stu_id==temp_issued->stu_id)
            {
                cur_stu->books_issued--;
                int date_diff=diff(temp_issued->return_date);
                if(date_diff>0)
                    cur_stu->fine+=(5*date_diff);
                break;
            }
            cur_stu=cur_stu->link_stu;
        }
        // file updating in book_dept.txt.
        FILE *fp_dept;
        fp_dept=fopen("book_dept.txt","r");
        FILE *temp_dept;
        temp_dept=fopen("temp_dept.txt","w");
        struct book_dept dept;
        if(fp_dept!=NULL)
            while(!feof(fp_dept))
            {
                fscanf(fp_dept,"%s%d%d",dept.dept_name,&dept.total,&dept.available);
                fscanf(fp_dept,"\n");
                if(strcmp(dept.dept_name,cur_book->dept)==0) // department matched
                    dept.available++;
                fprintf(temp_dept,"%s\t%d\t%d\n",dept.dept_name,dept.total,dept.available);
            }
        fclose(fp_dept);
        fclose(temp_dept);
        remove("book_dept.txt");
        rename("temp_dept.txt","book_dept.txt");
        printf("\n book returned successfully \n");
        free(temp_issued);
    }
}
/** function to print fine and books issued by a particular student based on student enroll id. **/
void student_details()
{
    int status=0;
    cur_stu=start_stu;
    cur_issued=start_issued;
    int temp_id;
    printf("\n enter student enroll id for getting details \n");
    scanf("%d",&temp_id);
    while(cur_stu!=NULL) // traverse the details of registered students
    {
        if(cur_stu->stu_id==temp_id)
        {
            status=1; // means student found in the system.
            // fine and  number of books issued printing
            printf("\n\n student name is %s \n fine is %d \n books issued is %d \n\n",cur_stu->stu_name,cur_stu->fine,cur_stu->books_issued);
            break;
        }
        cur_stu=cur_stu->link_stu;
    }
    if(status==0) // student with particular is not found in the system
        printf("\n student with provided enrollment id is not registered \n");
    else // here cur_stu points to the student details. // means status is 1
    {
        //printing of books name issued by student if any
        if(cur_stu->books_issued!=0)
        {
            printf("\n\n books issued by student are :\n");
            while(cur_issued!=NULL)
            {
                if(cur_issued->stu_id==cur_stu->stu_id)
                {
                    printf("book id=%d and book name=%s\n",cur_issued->book_id,cur_issued->book_name);
                }
                cur_issued=cur_issued->link_issued;
            }
        }
    }
}
/** function to calculate total,available,issued books **/
void num_book()
{
    FILE *fp_dept;
    fp_dept=fopen("book_dept.txt","r");
    if(fp_dept==NULL)
        printf("\n\n total books = 0 \n available books in library = 0 \n issued books = 0");
    else
    {
    struct book_dept dept;
    int issued_books=0;
    int total_books=0;
    int available_books=0;
    while(!feof(fp_dept))
    {
        fscanf(fp_dept,"%s%d%d",dept.dept_name,&dept.total,&dept.available);
        fscanf(fp_dept,"\n");
        total_books+=dept.total;
        available_books+=dept.available;
    }
    issued_books=total_books-available_books;
    printf("\n\n total books = %d \n available books in library = %d \n issued books = %d",total_books,available_books,issued_books);
    }
    fclose(fp_dept);
}
/** function for printing details of number of books based on department. **/
void dept_num()
{
    struct book_dept dept;
    FILE *fp_dept;
    fp_dept=fopen("book_dept.txt","r");
    if(fp_dept==NULL)
        printf("\n\n no books in the system.\n");
    else
    {
        printf("\n\n");
        while(!feof(fp_dept))
        {
           fscanf(fp_dept,"%s%d%d",dept.dept_name,&dept.total,&dept.available);
           fscanf(fp_dept,"\n");
           printf("dept = %s \t total installed books in  system = %d \t available in library = %d \n",dept.dept_name,dept.total,dept.available);
        }
    }
    fclose(fp_dept);
}
// function to find a book availability
void find_book()
{
    int count=0;
    int temp_id;
    char book_name[25];
    int status=0;
    int choice;
    cur_book=start_book;
    printf("\n\n enter choice in integer based on the following : \n");
    printf("\n1. based on id 2.based on name\n");
    printf("printing based on name will show number of books present with the same name case sensitive :\n");
    scanf("%d",&choice);
    switch(choice)
    {
    case 1:
        printf("\n enter book id to search :  ");
        scanf("%d",&temp_id);
        while(cur_book!=NULL)
        {
            if(cur_book->book_id==temp_id)
            {
                status=1;
                if(cur_book->status==1)
                    printf("\n book available for issue.\n");
                else
                    printf("\n book is already issued.\n"); // as already known that book is installed in library system.
                break;
            }
            cur_book=cur_book->link_book;
        }
        if(status==0)
            printf("\n book is not in library system. \n");
        break;
    case 2:
        printf("\n enter name of the book :  ");
        scanf("\n%[^\n]",book_name);
        while(cur_book!=NULL)
        {
            if(strcmp(cur_book->book_name,book_name)==0)
            {
                if(cur_book->status==1)
                {
                    status=1;
                    count++;
                }
            }
            cur_book=cur_book->link_book;
        }
        if(status==0)
            printf("\n book not found in library system. \n");
        else
            printf("\n number of books present in library with provided book name is :%d \n",count);
        break;
    default:
        printf("\n\n wrong input of choice \n");
        break;
    }
}
int main()
{
    char pass[55]="sdf";
    char password[55];
    printf("password : \n");
    int run=0;
    do
    {
        password[run]=getch();
        if(password[run]!='\r')
        {
            printf("*");
        }
        run++;
    }while(password[run-1]!='\r');
    password[run-1]='\0';
    if(strcmp(pass,password)==0)
    {
    getdata();
    int id_delete;
	int choice;
	printf("\n------------LIBRARY MANAGMENT SYSTEM-------------\n\n");
	do{
        printf("\n\n");
		printf("1. NEW STUDENT REGISTERED \n"); //NEW STUDENT NOW CAN NOW TAKE BENIFITS OF LIBRARY FACILITIES
		printf("2. REMOVE STUDENT FROM SYSYTEM \n"); //delete record of a particular student
		printf("3. ADD NEW BOOK \n");// addition of new book in library
		printf("4. DELETE BOOK FROM SYSTEM \n"); //deletion of a particular book from library
		printf("5. ISSUE A BOOK \n"); // issues book to a particular student
		printf("6. RETURN BOOK \n"); // returns book to the system and update information for a student and book
		printf("7. PRINT DETAILS OF STUDENT \n"); // prints fine and number of books issued.
		printf("8. PRINT NUMBER OF BOOKS \n"); // prints total number of books installed,issued books,available books in library.
		printf("9. PRINT BOOK DETAILS DEPARTMENT WISE \n"); // prints number of books total and available in library based on department.
		printf("10. SEARCH FOR A BOOK \n"); // search for a book if available for issuing or not.
		printf("11. SAVE AND EXIT\n\n"); // for saving data in files and exit
		printf("ENTER YOUR CHOICE  :");
		scanf("%d",&choice);
		switch(choice)
		{
        case 1:
            add_stu();
            break;
        case 2:
            delete_stu();
            break;
        case 3:
            add_book();
            break;
        case 4:
            delete_book();
            break;
        case 5:
            issue_book();
            break;
        case 6:
            return_book();
            break;
        case 7:
            student_details();
            break;
        case 8:
            num_book();
            break;
        case 9:
            dept_num();
            break;
        case 10:
            find_book();
            break;
        case 11:
            putdata();
            exit(0);
        default:
            printf("\nwrong input choice \n\t enter choice again=>from the following :  \n");
		}

      }while(choice!=11);
    }
    else
            printf("\n. wrong password input. \n");
}
struct date_str curdate()
{
  struct date_str cur_date;
  time_t now;
  time(&now);
  struct tm *tm=localtime(&now);
  cur_date.date=tm->tm_mday;
  cur_date.month=tm->tm_mon+1;
  cur_date.year=tm->tm_year +1900;
  return cur_date;
}
int countLeapYears(struct date_str d)
{
    int years = d.year;

    if (d.month <= 2)
        years--;

    return years / 4 - years / 100 + years / 400;
}
int offsetDays(int date, int month, int year)
{
  int offset = date;

  switch (month - 1)
  {
  case 11:
    offset += 30;
  case 10:
    offset += 31;
  case 9:
    offset += 30;
  case 8:
    offset += 31;
  case 7:
    offset += 31;
  case 6:
    offset += 30;
  case 5:
    offset += 31;
  case 4:
    offset += 30;
  case 3:
    offset += 31;
  case 2:
    offset += 28;
  case 1:
    offset += 31;
  }

  if (isLeap(year) && month > 2)
    offset += 1;

  return offset;
}
int getDifference(struct date_str dt1,struct date_str dt2)
{
    long int n1 = dt1.year*365 + dt1.date;

    for (int i=0; i<dt1.month - 1; i++)
        n1 += monthDays[i];

    n1 += countLeapYears(dt1);

    // SIMILARLY, COUNT TOTAL NUMBER OF DAYS BEFORE 'dt2'

    long int n2 = dt2.year*365 + dt2.date;
    for (int i=0; i<dt2.month - 1; i++)
        n2 += monthDays[i];
    n2 += countLeapYears(dt2);

    // return difference between two counts
    return (n1 - n2);
}
int diff(struct date_str return_date)
{
  struct date_str cur_date;
  cur_date=curdate();
  int year=getDifference(cur_date,return_date);
  // printf("%d",year);
}
int isLeap(int year)
{
  if (year%100 != 0 && year%4 == 0 || year %400 == 0)
    return 1;

  return 0;
}
void revoffsetDays(int offset, int year, int *date, int *mon)
{
  int month[13] = { 0, 31, 28, 31, 30, 31, 30,
          31, 31, 30, 31, 30, 31 };

  if (isLeap(year))
    month[2] = 29;

  int i;
  for (i = 1; i <= 12; i++)
  {
    if (offset <= month[i])
      break;
    offset = offset - month[i];
  }

  *date = offset;
  *mon = i;
}
struct date_str addDays(int date, int month, int year, int num)
{
  int offset1 = offsetDays(date, month, year);
  int remDays = isLeap(year)?(366-offset1):(365-offset1);
  struct date_str return_date;
  int offset2;
  if (num <= remDays)
  {
    return_date.year = year;
    offset2 = offset1 + num;
  }

  else
  {
    // x may store thousands of days.
    // We find correct year and offset
    // in the year.
    num -= remDays;
    return_date.year = year + 1;
    int y2days = isLeap(return_date.year)?366:365;
    while (num >= y2days)
    {
      num -= y2days;
      return_date.year++;
      y2days = isLeap(return_date.year)?366:365;
    }
    offset2 = num;
  }

  // Find values of day and month from
  // offset of result year.
    revoffsetDays(offset2, return_date.year,&return_date.date,&return_date.month);
    return return_date;
}
struct date_str returndate()
{
  struct date_str cur_date,return_date;
  time_t now;
  time(&now);
  struct tm *tm=localtime(&now);
  cur_date.date=tm->tm_mday;
  cur_date.month=tm->tm_mon+1;
  cur_date.year=tm->tm_year +1900;
  return_date=addDays(cur_date.date,cur_date.month,cur_date.year,15);
  return return_date;
}

