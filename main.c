
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX 100
#define ESCALATION_DAYS 3
#define DATA_FILE "complaints.txt"

// Structure to store complaint details
struct Complaint
{
    int id;
    int ward;
    int category;
    char description[200];
    char department[70];
    char status[20];
    long long registeredDate;
    long long resolvedDate;
    int isEscalated;
};

// Global variables
struct Complaint complaints[MAX];
int count = 0;
int nextComplaintID = 1001;


// Function declarations
int readInt(char message[]);
void readLine(char message[], char text[], int size);

void showCategories(void);
char* getCategoryName(int category);
char* assignDepartmentName(int category);

void loadComplaints(void);
void saveComplaints(void);

void registerComplaint(void);
void checkComplaintStatus(void);
void showAllComplaints(void);
void updateComplaintStatus(void);

void updateEscalations(void);
void showEscalationAlerts(void);

void showWardStatistics(void);
void showDepartmentStatistics(void);
void showStatistics(void);

int findComplaint(int id);

void citizenMenu(void);
void staffMenu(void);
void staffLogin(void);


// Reads an integer safely
int readInt(char message[])
{
    char input[100];
    char *end;
    long value;

    while (1)
    {
        printf("%s", message);

        if (fgets(input, sizeof(input), stdin) == NULL)
            return -1;

        value = strtol(input, &end, 10);

        while (*end == ' ' || *end == '\t' ||
               *end == '\n' || *end == '\r')
        {
            end++;
        }

        if (end != input && *end == '\0' &&
            value >= -2147483647L &&
            value <= 2147483647L)
        {
            return (int)value;
        }

        printf("Invalid input. Please enter a valid number.\n");
    }
}


// Reads a complete line of text
void readLine(char message[], char text[], int size)
{
    int length;
    int ch;

    printf("%s", message);

    if (fgets(text, size, stdin) == NULL)
    {
        text[0] = '\0';
        return;
    }

    length = strlen(text);

    if (length > 0 && text[length - 1] == '\n')
    {
        text[length - 1] = '\0';
    }
    else
    {
        while ((ch = getchar()) != '\n' && ch != EOF)
        {
            // Clear extra input
        }
    }
}


// Displays complaint categories
void showCategories(void)
{
    printf("\n========== COMPLAINT CATEGORIES ==========\n");

    printf("1. Water Supply\n");
    printf("2. Road Damage / Potholes\n");
    printf("3. Streetlight Failure\n");
    printf("4. Garbage Collection\n");
    printf("5. Drainage / Sewage Overflow\n");
    printf("6. Public Park Maintenance\n");
    printf("7. Stray Animal Issues\n");
    printf("8. Public Toilet Maintenance\n");
    printf("9. Traffic Signal Failure\n");
    printf("10. Illegal Dumping\n");
}


// Returns category name
char* getCategoryName(int category)
{
    switch (category)
    {
        case 1: return "Water Supply";
        case 2: return "Road Damage";
        case 3: return "Streetlight Failure";
        case 4: return "Garbage Collection";
        case 5: return "Drainage";
        case 6: return "Park Maintenance";
        case 7: return "Stray Animal Issues";
        case 8: return "Public Toilet";
        case 9: return "Traffic Signal";
        case 10: return "Illegal Dumping";
        default: return "Unknown";
    }
}


// Automatically assigns department based on category
char* assignDepartmentName(int category)
{
    switch (category)
    {
        case 1: return "Water Supply Department";
        case 2: return "Public Works Department";
        case 3: return "Electrical Department";
        case 4: return "Waste Management Department";
        case 5: return "Drainage Department";
        case 6: return "Parks Department";
        case 7: return "Animal Welfare Department";
        case 8: return "Public Health Department";
        case 9: return "Traffic Engineering Department";
        case 10: return "Environmental Enforcement Department";
        default: return "Unknown Department";
    }
}


// Saves complaints into a file
void saveComplaints(void)
{
    FILE *fp;
    int i;

    fp = fopen(DATA_FILE, "w");

    if (fp == NULL)
    {
        printf("Error saving complaint data.\n");
        return;
    }

    for (i = 0; i < count; i++)
    {
        fprintf(fp, "%d|%d|%d|%s|%s|%s|%lld|%lld|%d\n",
                complaints[i].id,
                complaints[i].ward,
                complaints[i].category,
                complaints[i].description,
                complaints[i].department,
                complaints[i].status,
                complaints[i].registeredDate,
                complaints[i].resolvedDate,
                complaints[i].isEscalated);
    }

    fclose(fp);
}


// Loads saved complaints when program starts
void loadComplaints(void)
{
    FILE *fp;
    char line[512];

    int id, ward, category, escalated;
    long long registered, resolved;

    char description[200];
    char department[70];
    char status[20];

    fp = fopen(DATA_FILE, "r");

    if (fp == NULL)
        return;

    while (fgets(line, sizeof(line), fp) != NULL &&
           count < MAX)
    {
        if (sscanf(line,
                   "%d|%d|%d|%199[^|]|%69[^|]|%19[^|]|%lld|%lld|%d",
                   &id, &ward, &category,
                   description, department, status,
                   &registered, &resolved, &escalated) == 9)
        {
            if (category < 1 || category > 10 ||
                ward <= 0 || id <= 0)
            {
                continue;
            }

            complaints[count].id = id;
            complaints[count].ward = ward;
            complaints[count].category = category;

            strcpy(complaints[count].description, description);
            strcpy(complaints[count].department, department);
            strcpy(complaints[count].status, status);

            complaints[count].registeredDate = registered;
            complaints[count].resolvedDate = resolved;
            complaints[count].isEscalated = escalated;

            if (id >= nextComplaintID)
                nextComplaintID = id + 1;

            count++;
        }
    }

    fclose(fp);
}


// Finds complaint using complaint ID
int findComplaint(int id)
{
    int i;

    for (i = 0; i < count; i++)
    {
        if (complaints[i].id == id)
            return i;
    }

    return -1;
}


// Registers a new complaint
void registerComplaint(void)
{
    int category;
    int ward;
    int i;

    char description[200];

    if (count >= MAX)
    {
        printf("\nComplaint storage is full.\n");
        return;
    }

    printf("\n========== REGISTER COMPLAINT ==========\n");

    showCategories();

    category = readInt("\nEnter category number: ");

    if (category < 1 || category > 10)
    {
        printf("Invalid category.\n");
        return;
    }

    ward = readInt("Enter ward number: ");

    if (ward <= 0)
    {
        printf("Ward number must be positive.\n");
        return;
    }

    readLine("Enter complaint description: ",
             description, sizeof(description));

    if (strlen(description) == 0)
    {
        printf("Description cannot be empty.\n");
        return;
    }

    // Replace file separator character if entered
    for (i = 0; description[i] != '\0'; i++)
    {
        if (description[i] == '|')
            description[i] = '/';
    }

    // Generate complaint ID automatically
    complaints[count].id = nextComplaintID++;

    complaints[count].ward = ward;
    complaints[count].category = category;

    strcpy(complaints[count].description, description);

    // Automatically route complaint to department
    strcpy(complaints[count].department,
           assignDepartmentName(category));

    strcpy(complaints[count].status, "Received");

    complaints[count].registeredDate = (long long)time(NULL);
    complaints[count].resolvedDate = 0;
    complaints[count].isEscalated = 0;

    count++;

    saveComplaints();

    printf("\nComplaint registered successfully!\n");
    printf("Complaint ID: %d\n", complaints[count - 1].id);
    printf("Category: %s\n", getCategoryName(category));
    printf("Department: %s\n", complaints[count - 1].department);
    printf("Status: Received\n");
}


// Allows citizen to check complaint status
void checkComplaintStatus(void)
{
    int id;
    int index;

    printf("\n========== CHECK COMPLAINT STATUS ==========\n");

    id = readInt("Enter complaint ID: ");

    index = findComplaint(id);

    if (index == -1)
    {
        printf("Complaint ID not found.\n");
        return;
    }

    printf("\nComplaint ID: %d\n", complaints[index].id);
    printf("Ward: %d\n", complaints[index].ward);

    printf("Category: %s\n",
           getCategoryName(complaints[index].category));

    printf("Description: %s\n", complaints[index].description);

    printf("Department: %s\n", complaints[index].department);

    printf("Status: %s\n", complaints[index].status);

    if (complaints[index].isEscalated)
        printf("Escalation: ESCALATED\n");
    else
        printf("Escalation: Not escalated\n");
}


// Displays all complaints for staff
void showAllComplaints(void)
{
    int i;

    printf("\n========== ALL COMPLAINTS ==========\n");

    if (count == 0)
    {
        printf("No complaints registered.\n");
        return;
    }

    for (i = 0; i < count; i++)
    {
        printf("\nComplaint ID: %d\n", complaints[i].id);
        printf("Ward: %d\n", complaints[i].ward);

        printf("Category: %s\n",
               getCategoryName(complaints[i].category));

        printf("Department: %s\n", complaints[i].department);

        printf("Description: %s\n", complaints[i].description);

        printf("Status: %s\n", complaints[i].status);

        if (complaints[i].isEscalated)
            printf("Escalation: ESCALATED\n");
        else
            printf("Escalation: Not escalated\n");

        printf("------------------------------------\n");
    }
}


// Updates complaint status
void updateComplaintStatus(void)
{
    int id;
    int index;
    int choice;

    printf("\n========== UPDATE COMPLAINT STATUS ==========\n");

    id = readInt("Enter complaint ID: ");

    index = findComplaint(id);

    if (index == -1)
    {
        printf("Complaint ID not found.\n");
        return;
    }

    if (strcmp(complaints[index].status, "Resolved") == 0)
    {
        printf("This complaint is already resolved.\n");
        return;
    }

    printf("\nCurrent status: %s\n", complaints[index].status);

    printf("\n1. Received\n");
    printf("2. Assigned\n");
    printf("3. In Progress\n");
    printf("4. Resolved\n");

    choice = readInt("Select new status: ");

    switch (choice)
    {
        case 1:
            strcpy(complaints[index].status, "Received");
            complaints[index].resolvedDate = 0;
            break;

        case 2:
            strcpy(complaints[index].status, "Assigned");
            complaints[index].resolvedDate = 0;
            break;

        case 3:
            strcpy(complaints[index].status, "In Progress");
            complaints[index].resolvedDate = 0;
            break;

        case 4:
            strcpy(complaints[index].status, "Resolved");
            complaints[index].resolvedDate = (long long)time(NULL);
            break;

        default:
            printf("Invalid status selection.\n");
            return;
    }

    saveComplaints();

    printf("\nComplaint status updated successfully!\n");
    printf("New status: %s\n", complaints[index].status);
}


// Automatically checks complaints for escalation
void updateEscalations(void)
{
    int i;
    int changed = 0;

    long long currentTime;
    double pendingDays;

    currentTime = (long long)time(NULL);

    for (i = 0; i < count; i++)
    {
        // Resolved complaints are not escalated
        if (strcmp(complaints[i].status, "Resolved") != 0)
        {
            pendingDays =
                difftime((time_t)currentTime,
                         (time_t)complaints[i].registeredDate)
                / 86400.0;

            if (pendingDays > ESCALATION_DAYS &&
                complaints[i].isEscalated == 0)
            {
                complaints[i].isEscalated = 1;
                changed = 1;
            }
        }
    }

    if (changed)
        saveComplaints();
}


// Displays escalated complaints
void showEscalationAlerts(void)
{
    int i;
    int found = 0;

    double pendingDays;

    updateEscalations();

    printf("\n========== ESCALATION ALERTS ==========\n");

    for (i = 0; i < count; i++)
    {
        if (complaints[i].isEscalated == 1 &&
            strcmp(complaints[i].status, "Resolved") != 0)
        {
            pendingDays =
                difftime(time(NULL),
                         (time_t)complaints[i].registeredDate)
                / 86400.0;

            printf("\nComplaint ID: %d\n", complaints[i].id);
            printf("Ward: %d\n", complaints[i].ward);

            printf("Department: %s\n", complaints[i].department);

            printf("Status: %s\n", complaints[i].status);

            printf("Pending days: %.1f\n", pendingDays);

            printf("ESCALATED: Requires attention!\n");

            printf("------------------------------------\n");

            found = 1;
        }
    }

    if (!found)
        printf("No pending escalated complaints.\n");
}


// Displays statistics for each ward
void showWardStatistics(void)
{
    int i, j;
    int total, resolved, pending, escalated;
    int alreadyDisplayed;

    updateEscalations();

    printf("\n========== WARD-WISE STATISTICS ==========\n");

    if (count == 0)
    {
        printf("No complaint data available.\n");
        return;
    }

    for (i = 0; i < count; i++)
    {
        alreadyDisplayed = 0;

        // Avoid displaying the same ward twice
        for (j = 0; j < i; j++)
        {
            if (complaints[j].ward == complaints[i].ward)
            {
                alreadyDisplayed = 1;
                break;
            }
        }

        if (alreadyDisplayed)
            continue;

        total = 0;
        resolved = 0;
        pending = 0;
        escalated = 0;

        for (j = 0; j < count; j++)
        {
            if (complaints[j].ward == complaints[i].ward)
            {
                total++;

                if (strcmp(complaints[j].status, "Resolved") == 0)
                    resolved++;
                else
                {
                    pending++;

                    if (complaints[j].isEscalated)
                        escalated++;
                }
            }
        }

        printf("\nWard: %d\n", complaints[i].ward);
        printf("Total complaints: %d\n", total);
        printf("Resolved: %d\n", resolved);
        printf("Pending: %d\n", pending);
        printf("Escalated: %d\n", escalated);

        printf("------------------------------------\n");
    }
}


// Displays statistics for each department
void showDepartmentStatistics(void)
{
    int i, j;
    int total, resolved, pending, escalated;

    updateEscalations();

    printf("\n========== DEPARTMENT-WISE STATISTICS ==========\n");

    if (count == 0)
    {
        printf("No complaint data available.\n");
        return;
    }

    // Each category is assigned to one department
    for (i = 1; i <= 10; i++)
    {
        total = 0;
        resolved = 0;
        pending = 0;
        escalated = 0;

        for (j = 0; j < count; j++)
        {
            if (complaints[j].category == i)
            {
                total++;

                if (strcmp(complaints[j].status, "Resolved") == 0)
                    resolved++;
                else
                {
                    pending++;

                    if (complaints[j].isEscalated)
                        escalated++;
                }
            }
        }

        printf("\nDepartment: %s\n",
               assignDepartmentName(i));

        printf("Total complaints: %d\n", total);
        printf("Resolved: %d\n", resolved);
        printf("Pending: %d\n", pending);
        printf("Escalated: %d\n", escalated);

        printf("------------------------------------\n");
    }
}


// Displays all complaint statistics
void showStatistics(void)
{
    printf("\n========== COMPLAINT STATISTICS ==========\n");

    showWardStatistics();
    showDepartmentStatistics();
}


// Citizen menu
void citizenMenu(void)
{
    int choice;

    while (1)
    {
        printf("\n========== CIVICCONNECT ==========\n");
        printf("1. Register Complaint\n");
        printf("2. Check Complaint Status\n");
        printf("3. Exit\n");

        choice = readInt("Enter your choice: ");

        switch (choice)
        {
            case 1:
                registerComplaint();
                break;

            case 2:
                checkComplaintStatus();
                break;

            case 3:
                return;

            default:
                printf("Invalid choice.\n");
        }
    }
}


// Staff dashboard
void staffMenu(void)
{
    int choice;

    while (1)
    {
        printf("\n========== STAFF DASHBOARD ==========\n");

        printf("1. View All Complaints\n");
        printf("2. Update Complaint Status\n");
        printf("3. View Escalation Alerts\n");
        printf("4. View Complaint Statistics\n");
        printf("5. Logout\n");

        choice = readInt("Enter your choice: ");

        switch (choice)
        {
            case 1:
                showAllComplaints();
                break;

            case 2:
                updateComplaintStatus();
                break;

            case 3:
                showEscalationAlerts();
                break;

            case 4:
                showStatistics();
                break;

            case 5:
                return;

            default:
                printf("Invalid choice.\n");
        }
    }
}


// Staff login
void staffLogin(void)
{
    char username[50];
    char password[50];

    printf("\n========== STAFF LOGIN ==========\n");

    readLine("Username: ", username, sizeof(username));
    readLine("Password: ", password, sizeof(password));

    if (strcmp(username, "staff") == 0 &&
        strcmp(password, "civic123") == 0)
    {
        printf("\nLogin successful!\n");
        staffMenu();
    }
    else
    {
        printf("\nInvalid username or password.\n");
    }
}


// Main function
int main(void)
{
    int choice;

    // Load existing complaints
    loadComplaints();

    // Check for complaints requiring escalation
    updateEscalations();

    printf("====================================\n");
    printf("       CIVICCONNECT SYSTEM\n");
    printf(" Complaint Routing and Escalation\n");
    printf("====================================\n");

    while (1)
    {
        printf("\n========== MAIN MENU ==========\n");

        printf("1. Citizen\n");
        printf("2. Staff Login\n");
        printf("3. Exit\n");

        choice = readInt("Enter your choice: ");

        switch (choice)
        {
            case 1:
                citizenMenu();
                break;

            case 2:
                staffLogin();
                break;

            case 3:
                saveComplaints();
                printf("\nThank you for using CivicConnect!\n");
                return 0;

            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}
