# CivicConnect: Complaint Routing and Escalation System

## Team
- Team Member 1: THRILOK SHEKAR (USN:1AT24EC166)
- Team Member 2: TEJAS V (USN:1AT24EC165)
- Track: C

## How to run
1. Download or clone this repository.
2. Open the C source file in Code::Blocks.
3. Build and run the program.
4. The program automatically creates `complaints.txt` to store complaint records.
5. For staff access:
   - Username: `staff`
   - Password: `civic123`

## Features that work
1. Register complaints with category, ward and description.
2. Automatically route complaints to the appropriate department.
3. Update complaint status: Received, Assigned, In Progress and Resolved.
4. Automatically flag unresolved complaints pending for more than 3 days when the program checks for escalation.
5. Display complaint statistics by ward and department.
6. Store complaint records in a file so they remain available after restarting the program.

## Sample run
Example workflow:

1. A citizen registers a water supply complaint in Ward 5.
2. The system generates a complaint ID.
3. The complaint is automatically assigned to the Water Supply Department.
4. Staff logs in and updates the complaint status to Assigned.
5. Staff views complaint statistics and escalation alerts.


