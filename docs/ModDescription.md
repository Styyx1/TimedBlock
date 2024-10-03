## Description:

A simple timed block implementation. If you block withing 0.33 seconds of pressing the block key, your block is successful and it will stagger the attacker + any enemy within a certain range. A timed block will prevent any damage dealt to you.

#### Settings:
- sBlockKey --> set your dual wield block key here if you use a dual wield blocking mod
- fStaggerDistance --> area to stagger enemies around you

The mod will look up the your assigned block key when you start the game or when you load it and everytime you close the settings menu. If you, for whatever reason, start the game via ``coc locationName`` from the main menu, you need to open the journal menu once and close it so the mod knows your block key.

## Install/Uninstall:
whenever you want, the magic effect from this mod lasts for 0.33 seconds and the perk is empty, so even if it lasts in your game, it won't do anything.

## Requirements:
[Address Library](https://www.nexusmods.com/skyrimspecialedition/mods/32444) for your game version

## Compatibilty:

Works with Skyrim 1.5.97, 1.6.1170 and **maybe** Skyrim VR (I can't confirm and the offset I used for VR is just a guess)

## Credits:
- [SeaSparrow](https://next.nexusmods.com/profile/Shekhinaga/about-me?gameId=1704) for the damage hook
- [po3](https://next.nexusmods.com/profile/powerofthree/mods?gameId=1704) for clib-util and specifically the hotkey handling of it
- [colinswrath](https://next.nexusmods.com/profile/colinswrath/about-me?gameId=1704) for blade and blunt and most of the things that are in my Utility.h
- [thirdeyesqueegee](https://next.nexusmods.com/profile/ThirdEye3301/about-me?gameId=1704) for the skse plugin template
- and everyone who worked on SKSE and CommonLib
