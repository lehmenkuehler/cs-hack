# Internal CS:GO Cheat

## Overview

Human-like aim assist, developed and maintained between 2016 and 2020 for a small circle of friends. Using it today would probably damage one's trust factor, if it were still private. It's config only, no menu.

Here is a [showcase video](https://www.youtube.com/watch?v=wSdxXN6IHc0) of the aim assist with various weapons.

Uses nlohmann's [JSON library](https://github.com/nlohmann/json) and offset files generated with frk1's [hazedumper](https://github.com/frk1/hazedumper).
Structs, class IDs, etc. were mostly found on the [UC Wiki](https://www.unknowncheats.me/wiki/UnKnoWnCheaTs_Game_Hacking_Wiki).

The program requires [config.cfg](config/config.cfg) and [offsets.json](config/offsets.json) to be located in the directory _%userprofile%/Documents/Anda/_. The offsets need continuous updating.


## Configuration Syntax Settings

The config allows general, weapon and weapon-class specific settings. To apply a setting to one weapon (or group), insert the weapon expression in front of the option. If no weapon is specified, the setting is applied globally. The more specific a setting is, the higher is its priority. Variant settings allow playing with different settings while holding a defined key. The default config mainly utilizes _Mouse 5 (MB2)_ for features that require holding a button.


#### Priority Order

Example: Every weapon's tracking speed is set to 0.10. This value is overwritten for assault rifles (0.35). When using the AK47 and holding the ALT key, the value is overwritten with 0.60.

```
AimAssist::TrackingSpeed 0.10
ASSAULT_RIFLE AimAssist::TrackingSpeed 0.35
AK47 VARIANT_KEY ALT AimAssist::TrackingSpeed 0.60
```

#### Expressions

Weapon expressions can be found in [weapon.cpp](hack/weapon.h) and key expressions in [parser.cpp](hack/parser.cpp).
Alternatively keys can be assigned with hexadecimal key codes as listed [here](https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes).

Example of the same expression:
```
SNIPER_RIFLE TriggerBot_Key ALT
SNIPER_RIFLE TriggerBot_Key 0x12
```

#### Configuration Datatypes

| Datatype | Description | Example |
| --- | --- | --- |
| INT | Integer Value | 0, 1, 2, 1337, ... |
| FLOAT | Decimal Value | 0.1, 1.1, 1.337, 3.14159, ... |
| BOOL | Truth Value | 0 or 1 |
| HEX | Hexadecimal Keycode | 0x01, 0x02, 0x0F, ... |
| STR | Character Sequence | "LMB", "ENTER", "INSERT", ... |
| RGBA | Color (R, G, B, a) | "0xffff00ff" (yellow at 100% alpha), ... |


### General Settings

<dl>
  <dt>LoadConfigKey [STR or HEX]</dt>
  <dd>Key to reload the config. Changes made to <i>config.cfg</i> can be applied in-game. An audio signal will confirm that the config has been loaded. Invalid settings can make the game crash as there is no error handling. Reloading the config resets temporary changes like a disabled feature.</dd>

  <dt>PanicKey [STR or HEX]</dt>
  <dd>Exits the game without any further confirmation.</dd>

  <dt>SyncBreak [INT]</dt>
  <dd>Pause between each iteration in milliseconds. Some features do not get updated permanently to enhance the performance. The optimal setting is machine specific.</dd>

  <dt>ActivateConsole [BOOL]</dt>
  <dd>The console shows if the config and offset files have been loaded.</dd>

  <dt>HideConsoleAfterStartup [BOOL]</dt>
  <dd>Hides the console five seconds after injection.</dd>
</dl>


### Shared Settings

<dl>
   <dt>On [BOOL]</dt>
   <dd>Enables or disables the feature it is used on. </dd>

   <dt>Key [STR or HEX]</dt>
   <dd>Determines which key is used to activate the feature. Valid arguments are either the predefined strings in <i>parser.cpp</i> or the hexadecimal key codes for Windows key events.</dd>

   <dt>HoldKey [BOOL]</dt>
   <dd>Boolean value defining whether pressing the key toggles the feature or if the key must be held down.</dd>

   <dt>PlaySound [BOOL]</dt>
   <dd>Plays a beep sound when the feature is activated (high pitch) or deactivated (low pitch) if set to 1.</dd>
</dl>


### Aim Assist Settings

<dl>
   <dt>FieldOfViewType [INT]</dt>
   <dd>Set to degree [0] or Source Engine units [1] (dynamic FOV). Determines how the field of view is measured. A dynamic field of view gets smaller for entities far away. A player model in CS:GO is about 70 Source Engine units high: Using a dynamic FOV with 70 units for example activates the aim assist within a diameter of the target's height, regardless of its distance.</dd>

   <dt>FieldOfView [FLOAT]</dt>
   <dd>Field of view of the aim assist.</dd>

   <dt>TrackingSpeed [FLOAT]</dt>
   <dd>Value between 0.0 and 1.0 defining the converging velocity when resting. A small value provides slow tracking when not shooting. Very high values make the tracking seem unnatural, especially on moving targets. Setting this value to 0.0 disables tracking when not shooting.</dd>

   <dt>TrackingSpeedFire [FLOAT]</dt>
   <dd>Value between 0.0 and 1.0 defining the converging velocity when shooting. Values are recommended to be higher than for the resting <i>TrackingSpeed</i> as more aim compensation is needed while firing due to recoil.</dd>

   <dt>CurbZone [FLOAT]</dt>
   <dd>Factor to set a range in which the tracking is decelerated (hit box size reference). Determines the zone inside which the tracking speed is decreased to not lock on the target but track it smoothly. Setting this value to 1.0 will start the deceleration on the edge of the hitbox. Setting it to 2.0 will start the deceleration within a radius of two hitboxes, and so on.</dd>

   <dt>Punch [FLOAT]</dt>
   <dd>Value between 0.0 and 1.0 that determines the amount of recoil being compensated. A value of 0.0 will not compensate any recoil, a value of 1.0 will compensate the full amount of recoil if the tracking speed is high enough to follow the spraying pattern.</dd>

   <dt>DistanceReduction [FLOAT]</dt>
   <dd>Factor that reduces tracking speed for afar targets. A value of 1.0 will slow and soften the tracking for targets according to their distance to the local player. Setting the factor to zero will keep the tracking speed high to allow strong recoil compensation on long range.</dd>

   <dt>MaximumVelocity [FLOAT]</dt>
   <dd>Maximum permitted velocity of the local player until the aim assist is active. Moving faster deactivates the aim assist (completely or partly, see <i>LinearVelocityMapping</i>). The moving velocity with a knife is 250.0 Source Engine units per second. The feature can be disabled by setting it to 300.0 or higher.</dd>

   <dt>LinearVelocityMapping [BOOL]</dt>
   <dd>Linearly reduces the tracking speed until <i>MaximumVelocity</i> is reached. Allows maximum tracking strength when standing still with a linear drop-off. The feature can be disabled by setting it to 0.</dd>

   <dt>ForceHeadshot [BOOL]</dt>
   <dd>Exclusively converge to a target's head. Otherwise selects the closest bone to the crosshair.</dd>

   <dt>ForceBodyAfterShots [INT]</dt>
   <dd>Number of shots fired after which the aim assist will exclusively target the chest. This feature neglects <i>ForceHeadshot</i> and can be used to increase hit chance on long range after initially converging to the head.</dd>

   <dt>JumpDelay [INT]</dt>
   <dd>Reaction time to track a jumping target (milliseconds). Value that determines the aim assist's tracking delay when a target jumps.</dd>

   <dt>CrouchDelay [INT]</dt>
   <dd>Reaction time to track a crouching target (milliseconds). Value that determines the aim assist's tracking delay when a target crouches.</dd>

   <dt>MovementPrediction [INT]</dt>
   <dd>Shifts the target's coordinates to its predicted position in a definable amount of time (milliseconds). Because there is no bullet speed in CS, this feature has no practical use, unless the tracking speed is set to a low value.</dd>

   <dt>TransferPause [INT]</dt>
   <dd>Pause between switching targets after a kill while spraying (milliseconds). Allows a smooth transition to the next target.</dd>

   <dt>FirePause [INT]</dt>
   <dd>Time of deactivation after the last bullet (milliseconds). Pauses the aim assist after shooting to make the tracking look more human.</dd>

   <dt>PauseRangeExtensionFactor [FLOAT]</dt>
   <dd>Time added to <i>FirePause</i>: t = t_firepause + range_factor * distance / Source Engine units. Increases the pause after firing a shot at afar targets.</dd>

   <dt>SpotDelay [INT]</dt>
   <dd>Activation delay after the target is spotted for the first time (milliseconds).</dd>

   <dt>ActivationSoftening [INT]</dt>
   <dd>Softening duration after the aim assist is activated with a key event.</dd>
</dl>


### Triggerbot Settings

<dl>
   <dt>KeySpraydown [STR or HEX]</dt>
   <dd>Determines a key that triggers a long burst instead of a classic triggerbot shooting pattern. This feature is intended to be used together with the aim assist when holding an angle against a close-range target with automatic weapons.</dd>

   <dt>Mode [INT]</dt>
   <dd>Set to incross [0], hitbox [1] or both combined (OR) [2]. The incross triggerbot uses a Source Engine function to check if a target is behind the player's crosshair and works reliably in most situations, but not always (for example when aiming through smoke). The hitbox triggerbot scans for the target's hitbox and can be used to shoot at targets behind objects if <i>VisibilityCheck</i> is turned off. The combined option uses both methods at the same time.</dd>

   <dt>PauseAfterShot [INT]</dt>
   <dd>Time before the next shot can be triggered (milliseconds).</dd>

   <dt>PauseRangeExtensionFactor [FLOAT]</dt>
   <dd>Extends the standard pause with increasing distance to the target (milliseconds per unit) to make the triggerbot shoot with lower frequency on long-range if greater than 0.0. Time added to <i>PauseAfterShot</i>: t = t_pause + range_factor * distance / Source Engine units.</dd>

   <dt>FakeShotDuration [INT]</dt>
   <dd>Time after a kill during which shots are still triggered (milliseconds). Determines whether the previous target has died and keeps shooting during the defined timespan.</dd>

   <dt>FireExtension [INT]</dt>
   <dd>Applies only for spraydowns: Minimum duration (milliseconds) of activation after a contact. Results in shots being fired even after leaving the target, depending on the weapon's RPM and the duration defined.</dd>

   <dt>MaximumDistance [FLOAT]</dt>
   <dd>Distance between local player and target required to trigger (Source Engine units).</dd>

   <dt>MaximumVelocity [FLOAT]</dt>
   <dd>Maximum permitted velocity of the local player until the triggerbot is active. The moving velocity with a knife is 250.0 Source Engine units per second. The feature can be disabled by setting it to 300.0 or higher.</dd>

   <dt>DisableWhileEnteringScope [BOOL]</dt>
   <dd>Turns the triggerbot off while changing to the scope.</dd>

   <dt>EnableInScopeOnly [BOOL]</dt>
   <dd>Disables the triggerbot when not scoped in.</dd>

   <dt>HeadOnly [BOOL]</dt>
   <dd>Reacts to heads only.</dd>

   <dt>FriendlyFire [BOOL]</dt>
   <dd>Enables the triggerbot for teammates.</dd>

   <dt>VisibilityCheck [BOOL]</dt>
   <dd>Checks if a target is visible.</dd>
</dl>

### Wallhack / Glow Settings

<dl>
   <dt>ToggleForAllWeapons [BOOL]</dt>
   <dd>Allows to apply toggling to all [1] or a single weapon [0].</dd>

   <dt>SpottedOnly [BOOL]</dt>
   <dd>Enables the glow only for already spotted targets.</dd>

   <dt>DistinctColors [BOOL]</dt>
   <dd>Enables different glow colors for each target. The colors can be defined using <i>RGBA_DistinctColors</i>.</dd>

   <dt>DistinctMixFactor [FLOAT]</dt>
   <dd>Value between 0.0 and 1.0 that is the weight of <i>RGBA_Hostile1HP</i> for the health dependent glow color determination.</dd>

   <dt>RGBA_Color [RGBA]</dt>
   <dd>Glow color of an object.</dd>

   <dt>RGBA_DistinctColors [RGBA]</dt>
   <dd>List of colors to use for the <i>DistinctColors</i> feature.</dd>
</dl>

### Aimbot Settings

<dl>
   <dt>FieldOfView [FLOAT]</dt>
   <dd>Field of view of the aimbot.</dd>

   <dt>SingleShot [BOOL]</dt>
   <dd>Fires one shot once a target has been locked on. Allows to one-tap targets by turning on <i>FireShotIfNoTargetFound</i>, turning off visibility checks and binding all features to one button, for example RMB.</dd>

   <dt>FireShotIfNoTargetFound [BOOL]</dt>
   <dd>Fires one shot, even if no target has been found.</dd>

   <dt>VisibilityCheck [BOOL]</dt>
   <dd>Checks if a target is visible.</dd>

   <dt>FriendlyFire [BOOL]</dt>
   <dd>Enables the aimbot for teammates.</dd>
</dl>

### Recoil Control System Settings

<dl>
   <dt>ReductionFactor [FLOAT]</dt>
   <dd>Value between 0.0 and 1.0, determining the strength of the compensation.</dd>

   <dt>SyncPause [INT]</dt>
   <dd>Pause between each iteration in milliseconds. The regular <i>SyncBreak</i> doesn't fully apply for the RCS.</dd>
</dl>
