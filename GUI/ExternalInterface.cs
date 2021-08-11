
using System.Runtime.InteropServices;

namespace HackMenu
{
    class ExternalInterface
    {
        //Struct to house all settings and to be passed to the dll
        [StructLayout(LayoutKind.Sequential)]
        public struct Settings
        {

            public bool aimOn;
            public float overAimFactor;
            public bool overaim;
            public float antiTransfer;
            public bool silentAim;
            public int boneId;//8 = head, 6 = chest
            public bool teamESP;
            public int enemyESP;//0 = off | 1 = always on | 2 = only if visible
            public bool bhopOn;
            public float rangeDecMult;
            public float fov;
            public bool triggerOn;
            public float recoilControlFactor;
            public bool antiFlash;
            public float aimTime;
            public bool radar;
            public int friendlyR;
            public int friendlyG;
            public int friendlyB;
            public int enemyR;
            public int enemyG;
            public int enemyB;
            public bool running;

        };

       
        // starts the main control process of the dll
        [DllImport("CSGOHackRedux")]
        public static extern void mainLoop();
        // initializes the required data for the hack to function
        [DllImport("CSGOHackRedux")]
        public static extern int init();
        // Updates the anti-flash data
        [DllImport("CSGOHackRedux")]
        public static extern void updateAntiFlash();
        // starts the loop used for conducting BHops
        [DllImport("CSGOHackRedux")]
        public static extern void doBhop();
        /*
         * Updates the settings within the DLL
         * @param Pointer to a settings struct containing desired settings
         */
        [DllImport("CSGOHackRedux")]
        public static extern void updateSettings(ref Settings newSettings);
    }
}
