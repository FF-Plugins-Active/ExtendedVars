# ExtendedVars
 
 # TARGET PLATFORM
 * It works all platforms which Unreal support. But we don't have Apple Mac computers. So, you have to create your own binaries for Apple ecosystem.
 
#FEATURES
* <b>Android Path Helper</b>: It gives real absolute path of file on Android.

* <b>Runtime Font Loading</b>: It is a container of UFont pointer. Because we have to release it and UFontFace when we want to close game.

* <b>Runtime Font Release</b>: You have to release your runtime imported fonts after closing your game to prevent GC issues.

* <b>Export Texture2D As Bitmap</b>

* <b>Widget To Texture Render Target 2D</b>

* <b>Texture2D To Colors Array</b>

* and other conversion functions about math and bytes.

# REFERANCES
* Runtime Font Loading and Releasing<br />
https://forums.unrealengine.com/t/how-do-you-dynamically-create-a-ufont-at-runtime-in-c/405643/5