# Customized Productivity app
A productivity app written in C++ using QT framework. The end goal is to have following features:
* Todo-list
* Habit tracker
* Weekly schedule 
* [Impossible List](https://impossiblehq.com/impossible-list/)
* List of upcoming events using Google Calender API

![Screenshot](https://imgur.com/zI7O3JL.png "Title")

# Improvements and bugs 
- [ ] List of upcoming events using Google Calender API
- [ ] Error messages upon failed read/writes
- [x] Clean-up
- [x] Styling 
- [ ] **Bug: When clicking exit on new subgoal dialog**
- [ ] Let user be able to delete entries

# Structure
JSON array is used to store data. Each "feature" has its own JSON array, the ID of some data is given by its index in the JSON array. 

Instead of classic Model-View-Controller architecture this application structures each "feature" as a class. To add a new feature to the application simply inherit from `Feature` and `QWidget`. The `Feature` class holds the JSON array as a protected member `__database`, and provides method for saving the database. Furthermore a virtual method `v_display` must be overriden which handles the drawing of GUI elements. Inside `v_display` each feature must add its GUI elements to the parent widget which is a tab-widget. 





