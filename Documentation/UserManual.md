# Polybius Modtools User Manual

## Launcher

## Main App : Eu4

### Map movement

- Zoom in and out : ctl + mouse roll.
- Move Map : middle mouse button + drag, scrollbars, roll for vertical scroll.

### Province Selection

- select one province : left mouse button.
- select multiple provinces : right mouse button. 
- when selecting one province it resets the multiple province selection.

### Modifying province data

1. Go into Province Tab and change some data. This change is then put into a buffer.
2. Go into BufferedChanges Tab and either save or cancel these changes. Saving creates the new files while cancelling removes the changes from the buffer.

### Province Tab

Contains all widgets used for modifying a province data(history).

#### Technical Info

- Contains province metadata : ID, Name, RGB value and if its water tile.

#### Province Information

- Development : three ways to change data.
    1. Fixed value directly changing the number with keyboard or the arrows incrementing or decrementing by one.
    2. Add dev all and Sub dev all buttons incrementing or decrementing tax, prod, manpower values by one.
    3. Rnd low, Mid, High dev buttons which randomly changes all values in a range.
        - Low range : (1,3)
        - Mid range : (2,8)
        - High Range : (7,15)
- Culture and Religion :

    - Use the dropbox to select the new value. Can also type the first few letters to filter.
    - To delete the value select the empty field.
- OwnerShip, change owner and controller values :
    - Use the dropbox to select the new value. Can also type the first few letters to filter.
    - To delete the value select the empty field.
- Cores :
    - Add a core :
        1. Use the dropbox to select a country.
        2. Press Add core button.
        3. Result : a new line will show up in the widget with the corresponding tag.
    - Delete a core :
        1. Inside the table press the little `x` on the same line of the tag you want to delete inside the Delete column.
- Trade :
    - Change Trade Good :
        1. Use the dropbox to select a new trade good.
- Notice : in all dropbox you can delete the value but eu4 usually requires them unless its a wasteland, a sea tile or an uncolonized province. Please be careful deleting entries.

### BufferedChangesTab

This tab is used to visually represent the data changed in each file using a table. The Scheduled to be Deleted column indicates if the field will be deleted or not when saving.

- Batch save and cancel all changes:
    1. Press the corresponding button
- Cancel or save per file :
    1. Press the corresponding button inside the box representing the file.