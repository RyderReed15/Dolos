using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static HackMenu.Form1;
using static HackMenu.ExternalInterface;
using System.Drawing;
using System.Threading;
using System.Media;

namespace HackMenu
{
    class Controller
    {
        public static Dictionary<string, int> Bones = new Dictionary<string, int>();
        public static bool settingsChanged = false;

        public static Color Error = Color.FromArgb(255, 0, 0);
        public static Color Success = Color.FromArgb(0, 255, 0);

        public static Settings currentSettings;

        enum STATUS : int { STATUS_NO_GAME, STATUS_SUCESS, STATUS_NO_DRIVER } // DLL status outputs
        //Initalizes the bone dictionary
        public static void initBones()
        {

            Bones.Add("Head", 8);
            Bones.Add("Neck", 7);
            Bones.Add("Body", 6);
            Bones.Add("Right Hand", 39);
            Bones.Add("Left Hand", 13);
            Bones.Add("Right Leg", 73);
            Bones.Add("Left Leg", 66);

        }
        // Starts up the Dll
        public static void start(Form1 form)
        {
            currentSettings.running = true;
            form.outputSuccess("Successfully Initialized");
            STATUS status = (STATUS)init();
            if (status == STATUS.STATUS_SUCESS)
            {
                //Init

             
                Thread mainThread = new Thread(new ThreadStart(mainLoop));
                mainThread.Start();
                Thread loopThread = new Thread(new ThreadStart(updateLoop));
                loopThread.Start();
            }
            else
            {
                //Failed
                SystemSounds.Exclamation.Play();
                
                if (status == (int)STATUS.STATUS_NO_GAME)
                {
                    form.outputFailure("Failed To Attach To Game");
                }
                else if (status == STATUS.STATUS_NO_DRIVER)
                {
                    form.outputFailure("Failed To Connect To Driver");
                }
            }

        }
        //Loop to check for updates to cheat settings
        public static void updateLoop()
        {
            while (currentSettings.running)
            {

                Thread.Sleep(1000);
                updateWrapper();

            }

        }
        //Updates the cheat settings
        public static void updateWrapper()
        {
            if (settingsChanged)
            {
                settingsChanged = false;
                updateSettings(ref currentSettings);
            }

        }
        //Returns either on or off depending on boolean value
        public static string onOff(bool value)
        {
            if (value) return "ON";
            return "OFF";
        }


    }
}
