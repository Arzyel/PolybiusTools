# Bugs

## To Fix
- In culture dropbox there are incorrect fields : province and country(after korean).
## Known but no real solution
- In continents overlay, central africa(id:1796) and Alps6(id:4763) don't have the right color. This is caused by them not having a history province file which my architecture relies on to create the province correctly. For custom provinces just ensure they have an history file even if empty for wastelands.
- Again in continents overlay. Some sea tiles dont have the correct shade of blue. This indicates their Id's are found in the continents.txt file.
