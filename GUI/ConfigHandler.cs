

using static HackMenu.Controller;
using System.Configuration;

namespace HackMenu
{
    class ConfigHandler
    {
        static Configuration configManager;
        static KeyValueConfigurationCollection confCollection;

        /*
         * Loads a config file for the cheat settings
         * @param Full path of the desired config file
         */
        public static void loadConfig(string fileName)
        {

            ExeConfigurationFileMap configMap = new ExeConfigurationFileMap();
            configMap.ExeConfigFilename = fileName;
            configManager = ConfigurationManager.OpenMappedExeConfiguration(configMap, ConfigurationUserLevel.None);
            confCollection = configManager.AppSettings.Settings;

            currentSettings.aimOn = bool.Parse(confCollection["aimbotActive"].Value);
            currentSettings.radar = bool.Parse(confCollection["radarActive"].Value);
            currentSettings.bhopOn = bool.Parse(confCollection["bhopActive"].Value);
            currentSettings.antiFlash = bool.Parse(confCollection["antiflashActive"].Value);
            currentSettings.fov = float.Parse(confCollection["aimbotFOV"].Value);
            currentSettings.rangeDecMult = float.Parse(confCollection["fovDecMult"].Value);
            currentSettings.teamESP = bool.Parse(confCollection["teamESP"].Value);
            currentSettings.enemyESP = int.Parse(confCollection["enemyESP"].Value);
            currentSettings.triggerOn = bool.Parse(confCollection["triggerActive"].Value);
            currentSettings.recoilControlFactor = float.Parse(confCollection["recoilControlFactor"].Value);
            currentSettings.boneId = int.Parse(confCollection["boneId"].Value);
            currentSettings.aimTime = float.Parse(confCollection["aimTime"].Value);
            currentSettings.overaim = bool.Parse(confCollection["overaimActive"].Value);
            currentSettings.overAimFactor = float.Parse(confCollection["overaimFactor"].Value);
            currentSettings.silentAim = bool.Parse(confCollection["silentAimActive"].Value);
            currentSettings.antiTransfer = float.Parse(confCollection["antitransferAmount"].Value);

            currentSettings.friendlyR = int.Parse(confCollection["friendlyR"].Value);
            currentSettings.friendlyG = int.Parse(confCollection["friendlyG"].Value);
            currentSettings.friendlyB = int.Parse(confCollection["friendlyB"].Value);

            currentSettings.enemyR = int.Parse(confCollection["enemyR"].Value);
            currentSettings.enemyG = int.Parse(confCollection["enemyG"].Value);
            currentSettings.enemyB = int.Parse(confCollection["enemyB"].Value);

        }
        /*
         * Saves a new config file 
         * @param Full path to the desired save location
         */
        public static void saveConfig(string fileName)
        {
            confCollection["aimbotActive"].Value = currentSettings.aimOn.ToString();
            confCollection["radarActive"].Value = currentSettings.radar.ToString();
            confCollection["bhopActive"].Value = currentSettings.bhopOn.ToString();
            confCollection["antiflashActive"].Value = currentSettings.antiFlash.ToString();
            confCollection["aimbotFOV"].Value = currentSettings.fov.ToString();
            confCollection["fovDecMult"].Value = currentSettings.rangeDecMult.ToString();
            confCollection["teamESP"].Value = currentSettings.teamESP.ToString();
            confCollection["enemyESP"].Value = currentSettings.enemyESP.ToString();
            confCollection["triggerActive"].Value = currentSettings.triggerOn.ToString();
            confCollection["recoilControlFactor"].Value = currentSettings.recoilControlFactor.ToString();
            confCollection["boneId"].Value = currentSettings.boneId.ToString();
            confCollection["aimTime"].Value = currentSettings.aimTime.ToString();

            confCollection["overaimActive"].Value = currentSettings.overaim.ToString();
            confCollection["overaimFactor"].Value = currentSettings.overAimFactor.ToString();
            confCollection["silentAimActive"].Value = currentSettings.silentAim.ToString();
            confCollection["antitransferAmount"].Value = currentSettings.antiTransfer.ToString();

            confCollection["friendlyR"].Value = currentSettings.friendlyR.ToString();
            confCollection["friendlyG"].Value = currentSettings.friendlyG.ToString();
            confCollection["friendlyB"].Value = currentSettings.friendlyB.ToString();
            confCollection["enemyR"].Value = currentSettings.enemyR.ToString();
            confCollection["enemyG"].Value = currentSettings.enemyG.ToString();
            confCollection["enemyB"].Value = currentSettings.enemyB.ToString();
          
            configManager.SaveAs(fileName, ConfigurationSaveMode.Modified);
        }
        /*
         * Saves the currently active config file
         */
        public static void saveConfig()
        {
            confCollection["aimbotActive"].Value = currentSettings.aimOn.ToString();
            confCollection["radarActive"].Value = currentSettings.radar.ToString();
            confCollection["bhopActive"].Value = currentSettings.bhopOn.ToString();
            confCollection["antiflashActive"].Value = currentSettings.antiFlash.ToString();
            confCollection["aimbotFOV"].Value = currentSettings.fov.ToString();
            confCollection["fovDecMult"].Value = currentSettings.rangeDecMult.ToString();
            confCollection["teamESP"].Value = currentSettings.teamESP.ToString();
            confCollection["enemyESP"].Value = currentSettings.enemyESP.ToString();
            confCollection["triggerActive"].Value = currentSettings.triggerOn.ToString();
            confCollection["recoilControlFactor"].Value = currentSettings.recoilControlFactor.ToString();
            confCollection["boneId"].Value = currentSettings.boneId.ToString();
            confCollection["aimTime"].Value = currentSettings.aimTime.ToString();

            confCollection["overaimActive"].Value = currentSettings.overaim.ToString();
            confCollection["overaimFactor"].Value = currentSettings.overAimFactor.ToString();
            confCollection["silentAimActive"].Value = currentSettings.silentAim.ToString();
            confCollection["antitransferAmount"].Value = currentSettings.antiTransfer.ToString();

            confCollection["friendlyR"].Value = currentSettings.friendlyR.ToString();
            confCollection["friendlyG"].Value = currentSettings.friendlyG.ToString();
            confCollection["friendlyB"].Value = currentSettings.friendlyB.ToString();
            confCollection["enemyR"].Value = currentSettings.enemyR.ToString();
            confCollection["enemyG"].Value = currentSettings.enemyG.ToString();
            confCollection["enemyB"].Value = currentSettings.enemyB.ToString();

            configManager.Save(ConfigurationSaveMode.Modified);
        }

    }
}
