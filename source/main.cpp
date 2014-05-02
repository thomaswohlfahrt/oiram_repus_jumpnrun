#include <cstdlib>
#include <iostream>
#include <SDL/SDL.h>

#include "application.h"

// needed for mingw32 because of >whatever<
//      *problems with SDLs own main funktion...
//          *maybe not the most complete Media Library available
#undef main
/**
\mainpage The mainpage documentation

\htmlonly

<font size="11">
 Dependencies
</font>
<br>
This project depends on the SDL libraries.
<br>
These include SDL1.2 SDL_image SDL_ttf and SDL_mixer.

<br>
Ubuntu and derivates:

<br>
<tt> sudo su

<br>
apt-get install libsdl1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev libsdl-ttf2.0-dev
</tt>

<br>
Under Windows you should add the appropriate libraries to your build environment such as MinGW.


<font size="11">
<br>
Building
</font>

<br>
<br>
To build the project you should link with the necessary developer libraries.
<br>

<tt> g++ *.cpp -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -I . -o Oiram\ Repus</tt>
<br>

<br>
<font size="11">
        Playing:
</font>

<br>
<br>
GNU/Linux:

<br>
<tt>./Oiram\ Repus</tt>

<br>
Windows:

<br>
Double click the built executable

<br>
<font size="11">
        Controls:
</font>

<ul>
<li> Arrowkeys for movement and jumping</li>

<li> Spacebar to shoot when eligible</li>
</ul>

<br>
<font size="11">Saving and Loading:</font>
<ul>
<li>  Highscores are saved, so you can show your friends how awesome you are!</li>

<li> Campaign progress is saved except map was modified!</li>

<li> All positions of enemies are saved along with the player information!</li>

<li>  Intuitive user interface utilizing mouse!</li>
</ul>


<font size="11">Integrated Leveleditor:</font>
<ul>
<li> Create exciting new challenges to have fun with your friends!</li>

<li> Load existing maps and modify them! True freedom.</li>

<li> Save your map and load it again in a menu, if you feel like it.</li>

<li> Display Help</li>
</ul>

<font size="11">Game caveats:</font>
<ul>
<li> The princess will try to shoot you, be careful!</li>

<li>Terrible hordes of enemies await! Crabs, Ghosts and the feared Grrr!</li>

<li>Fight off the unholy creatures with powerful attacks such as jumping and shooting</li>

<li>To switch into overdrive mode you can collect mighty artifacts!</li>

<li>Collect the star to gain the ability to incinerate your enemies with mighty balls of fire</li>

<li> Collect the heart to restore your essence of life</li>

<li> Collect the valuable gold coins to , .... well just collect the gold coins!</li>

<li> But beware, should you be struck down by the minions of the princess you will respawn at your starting position!</li>
</ul>

\endhtmlonly

If you specified `INPUT` or `FILE_PATTERNS` in your Doxyfile please
add `.dox` to your file patterns or `mainpage.dox` to your INPUT files.


</B>*/
int main(){

   Application app;
   app.exec();
   return 0;

}
