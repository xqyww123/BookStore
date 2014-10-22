Simple Book Store
---
## Install
Boost lib is used,
please indicate your boost lib path in visual studio project property page.
Boost version : 1.56.0


## Usage
Just simple type '?' at any problem.

Default Login access:

you could use _admin_ with password _admin_ to login into system with administrator authority, or _staff_ with _staff_ password to get staff authority.
you also could use _port/db_ file, which is my db file.


## UI framwork
The program implents a simple status machine.
Each status has some action, and Everytime the program only stay on a certain status.
Through the action, the program core code is invoked and status could be transformed.

For first time, status is HomePage, which has follow action:
- login
- register

After login, status transform to User Page, which has follow action:
- buy
- order : require system order new copys.
- add : Add some book into myself's reposity, only administrator authorized for the action.
- return : return a copy to system
- charge : charge money, only administrator authorized. For normal users, you should send money to admins, and admin will charge your money through User Manager.
- seek : show your self information.
- list : Print your copys.
- look\_all : Print all books in system.
- to\_book\_mng : transform current status to Book Manager, only staff authorized.
- to\_user\_mng : transform current status to User Manager, only administrator authorized.

User Mangager Status:
- grant : Change specified user's role.
- charge : Charge specified user's money.
- reigster : Register user into system.

Book Manager Status:
- order : add new copy of specified book.
- register : register new book into system.

## Role control
The store has three base role:
- Administrator
- Staff
- Guest

Role could be add using logic or operation.

Most of actions require enough authority, and they will check in the core code.
The reason why checking in core rather than in ui is that user may though client or any other terminal login to system, in which case ui check may be bypassed.

Administrator could use User Manager to change other's role.

## OO Concept
In the main core, the classes are defined as :
- Book class
- Copy class
Those are base class.

Then *BookRep class* which means a reposity of books and copys, assemble both two class.

Each *User* and *BookMng* all have a BookRep property, means their books and copys.

*User* also takes apart in role control. Each *User* have a Role property.

All User are managed by User Manager, whose access could only be accquired by administrator.
All Book and Copys are managed by Book Manager.
At last, *BookMng*, *UserMng*, *RoleMng* and *StorageMng* (which provide simple storage support) consists Managers.

Final, Core include all manager directly, control life cycle, build and invoke ui framework.

So, at the main function, I just simply call:
	Core::Prepare()
	Core::Launch()
	Core::Terminate()

That's all

## Author
Xero Essential (Xu Qiyuan 3130000950)
