# Riddlebit Analytics
Riddlebit Analytics is a game analytics plugin for Unreal Engine 4. It is supposed to be used with a compatible back-end for storage and queries.

## Installation
To integrate this plugin into your unreal engine project, simply clone this repository into your Plugins folder (if you don't have one, make one), so your project structure looks something like this:

    YourProjectRoot
    +-- Content
    +-- Source
    +-- Config
    +-- Plugins
    |   +-- RiddlebitAnalytics 

Then compile the project before using the plugin. If done successfully, you can go into any blueprint and search for the "InitAnalytics" node, and if it shows up, you're good to go!

## Development
This plugin was primarily developed for internal use, but it will be updated and maintained whenever possible. At the moment both the uploading and downloading of data is somewhat tied together, but in the future this will be restructured in a clean way. 👁