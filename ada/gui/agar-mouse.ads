------------------------------------------------------------------------------
--                             AGAR GUI LIBRARY                             --
--                           A G A R  . M O U S E                           --
--                                 S p e c                                  --
------------------------------------------------------------------------------
with Interfaces;
with Interfaces.C;
with Interfaces.C.Strings;
with Agar.Input_Device;

package Agar.Mouse is
  package C renames Interfaces.C;
  package CS renames Interfaces.C.Strings;
  package INDEV renames Agar.Input_Device;

  use type C.int;
  use type C.unsigned;

  ------------------
  -- Mouse Button --
  ------------------
  type Mouse_Button is
    (NONE,
     LEFT,
     MIDDLE,
     RIGHT,
     WHEEL_UP,
     WHEEL_DOWN,
     X1,
     X2,
     ANY);
  for Mouse_Button use
    (NONE       => 16#00_00#,
     LEFT       => 16#00_01#,
     MIDDLE     => 16#00_02#,
     RIGHT      => 16#00_03#,
     WHEEL_UP   => 16#00_04#,
     WHEEL_DOWN => 16#00_05#,
     X1         => 16#00_06#,
     X2         => 16#00_07#,
     ANY        => 16#00_ff#);
  for Mouse_Button'Size use C.unsigned'Size;
  
  -------------------------
  -- Mouse Button Action --
  -------------------------
  type Mouse_Button_Action is
    (PRESSED,
     RELEASED);
  for Mouse_Button_Action use
    (PRESSED  => 0,
     RELEASED => 1);
  for Mouse_Button_Action'Size use C.int'Size;
  
  ------------------
  -- Mouse Device --
  ------------------
  type Mouse_Device is limited record
    Super        : aliased INDEV.Input_Device; -- [Input_Device -> Mouse]
    Button_Count : C.unsigned;                 -- Button count (or 0)
    Button_State : Mouse_Button;               -- Last button state
    X,Y          : C.int;                      -- Last cursor position
    Xrel, Yrel   : C.int;                      -- Last relative motion
  end record
    with Convention => C;

  type Mouse_Device_Access is access all Mouse_Device with Convention => C;
  subtype Mouse_Device_not_null_Access is not null Mouse_Device_Access;

end Agar.Mouse;
