# Icecream-Factory-Problem-OS
## [CS2006 Operating Systems] SEMESTER PROJECT
### TEAM MEMBERS: 
      21K-4578(SAMAHA BATOOL); 
      21K-4897(LAIBA MEER); 
      21K-3301(SHAYAN LIAQAT);
### TEACHER:
      DR. NADEEM KAFI

### LINUX VERSION:
      4.19.282
### PROGRAMMING LANGUAGE:
      C
### FILES INCLUDED: 
      icecreamShop.c; 
      data.txt; 
      Makefile (makefile of system call); 
      project.c (system call file);

-------------------------------------------------------------------------------------------------------------------------------------------------------
The problem states to simulate an ice cream shop and cater to multiple customers while avoiding race conditions. It will manage the different processes involved in serving customers, such as taking orders, preparing the ice cream, and delivering it to the customers. The program will use synchronization techniques to avoid race conditions and ensure that the processes execute in the correct order.

The program is designed to handle multiple customers concurrently,allowing several customers to place orders simultaneously and customize their orders according to Size, Flavour, Toppings, Serving Mode (Cup or Cone). The program uses semaphores to synchronize the access to shared resources, such
as the order queue or the ice cream machines.

The system call is used to handle the communication between processes and to manage the shared resources such as the order queue or the ice cream machines.
Time Constraint is also introduced, such as a time limit for preparing an order or a maximum wait time for customers.

The data of customers is then stored in a txt file to keep record of ice-cream orders.

---------------------------------------------------------------------------------------------------------------------------------------------------
### WORK DIVISION
#### Samaha:
Implement the system call to handle communication between processes and manage shared resources.
Modify the order queue component to handle the new complexities in the order management system, including customer customizations and multiple flavors/sizes of ice cream.
Add time limit functionality to the order queue component to ensure timely preparation of orders.

#### Laiba:
Test and debug the system call implementation to ensure proper communication and synchronization among threads.
Modify the order processing component to handle the new complexities in the order management system, including customer customizations and multiple flavors/sizes of ice cream.
Modify the order processing component to handle time constraints and ensure prompt delivery of orders.
Test and debug the synchronization mechanisms to ensure that they work correctly with the new modifications.

#### M.Shayan:
Modify the ice cream making and delivery components to handle the new complexities in the order management system, including customer customizations and multiple flavors/sizes of ice cream.
Modify the delivery service component to handle time constraints and ensure prompt delivery of orders.
Test and debug the components to ensure that they work correctly with the new modifications.
Modify the thread creation and management functions to include the use of the system call for synchronization.
--------------------------------------------------------------------------------------------------------------------------------------------------------
