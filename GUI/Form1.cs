using System;
using System.Threading;
using System.Windows.Forms;
using System.Media;
using static HackMenu.ExternalInterface;
using static HackMenu.ConfigHandler;
using static HackMenu.Controller;



namespace HackMenu
{
    
    
    public partial class Form1 : Form
    {
               
        public Form1()
        {
            InitializeComponent();
            initBones();
            loadConfig(Properties.Settings.Default.ConfigPath);
            refreshValues();
            makeGraph();

        }

        private void Form1_Load(object sender, EventArgs e)
        {
            BoneList.SelectedIndex = 0;

            AimPanel.BackColor = Properties.Settings.Default.BackColorTwo;
            MiscPanel.BackColor = Properties.Settings.Default.BackColorTwo;
            ConfigPanel.BackColor = Properties.Settings.Default.BackColorTwo;
            WallsPanel.BackColor = Properties.Settings.Default.BackColorTwo;

            SideMenuPanel.BackColor = Properties.Settings.Default.BackColor;
            AimMenu.BackColor = Properties.Settings.Default.BackColor;
            WallMenu.BackColor = Properties.Settings.Default.BackColor;
            MiscMenu.BackColor = Properties.Settings.Default.BackColor;
            ConfigMenu.BackColor = Properties.Settings.Default.BackColor;
            DriverInject.BackColor = Properties.Settings.Default.BackColor;
            StartButton.BackColor = Properties.Settings.Default.BackColor;
            BoneList.BackColor = Properties.Settings.Default.BackColor;
            Refresh.BackColor = Properties.Settings.Default.BackColor;


            AimMenu.ForeColor = Properties.Settings.Default.ForeColor;
            WallMenu.ForeColor = Properties.Settings.Default.ForeColor;
            MiscMenu.ForeColor = Properties.Settings.Default.ForeColor;
            ConfigMenu.ForeColor = Properties.Settings.Default.ForeColor;
            DriverInject.ForeColor = Properties.Settings.Default.ForeColor;
            StartButton.ForeColor = Properties.Settings.Default.ForeColor;
            Refresh.ForeColor = Properties.Settings.Default.ForeColor;



            AimbotButton.BackColor = Properties.Settings.Default.BackColorTwo;
            OveraimButton.BackColor = Properties.Settings.Default.BackColorTwo;
            SilentAimButton.BackColor = Properties.Settings.Default.BackColorTwo;
            TeamWallButton.BackColor = Properties.Settings.Default.BackColorTwo;
            EnemyWallButton.BackColor = Properties.Settings.Default.BackColorTwo;
            TeamColorButton.BackColor = Properties.Settings.Default.BackColorTwo;
            EnemyColorButton.BackColor = Properties.Settings.Default.BackColorTwo;
            BHopButton.BackColor = Properties.Settings.Default.BackColorTwo;
            RadarButton.BackColor = Properties.Settings.Default.BackColorTwo;
            TriggerButton.BackColor = Properties.Settings.Default.BackColorTwo;
            AntiFlashButton.BackColor = Properties.Settings.Default.BackColorTwo;
            LoadConf.BackColor = Properties.Settings.Default.BackColorTwo;
            SaveConf.BackColor = Properties.Settings.Default.BackColorTwo;
            SaveNewConf.BackColor = Properties.Settings.Default.BackColorTwo;

            AimTimeText.BackColor = Properties.Settings.Default.BackColorTwo;
            AimTimeNum.BackColor = Properties.Settings.Default.BackColorTwo;
            OveraimText.BackColor = Properties.Settings.Default.BackColorTwo;
            OveraimFactor.BackColor = Properties.Settings.Default.BackColorTwo;
            AntiTransfer.BackColor = Properties.Settings.Default.BackColorTwo;
            AntitransferText.BackColor = Properties.Settings.Default.BackColorTwo;
            FOV.BackColor = Properties.Settings.Default.BackColorTwo;
            RangeDec.BackColor = Properties.Settings.Default.BackColorTwo;
            FovText.BackColor = Properties.Settings.Default.BackColorTwo;
            RangeDecFactor.BackColor = Properties.Settings.Default.BackColorTwo;
            FovChart.BackColor = Properties.Settings.Default.BackColorTwo;
            BoneID.BackColor = Properties.Settings.Default.BackColorTwo;
            panel3.BackColor = Properties.Settings.Default.BackColorTwo;
            panel4.BackColor = Properties.Settings.Default.BackColorTwo;
            RecoilControlText.BackColor = Properties.Settings.Default.BackColorTwo;
            FovChart.ChartAreas[0].BackColor = Properties.Settings.Default.BackColorTwo;

            AimbotButton.ForeColor = Properties.Settings.Default.ForeColor;
            OveraimButton.ForeColor = Properties.Settings.Default.ForeColor;
            SilentAimButton.ForeColor = Properties.Settings.Default.ForeColor;
            TeamWallButton.ForeColor = Properties.Settings.Default.ForeColor;
            EnemyWallButton.ForeColor = Properties.Settings.Default.ForeColor;
            TeamColorButton.ForeColor = Properties.Settings.Default.ForeColor;
            EnemyColorButton.ForeColor = Properties.Settings.Default.ForeColor;
            BHopButton.ForeColor = Properties.Settings.Default.ForeColor;
            RadarButton.ForeColor = Properties.Settings.Default.ForeColor;
            TriggerButton.ForeColor = Properties.Settings.Default.ForeColor;
            AntiFlashButton.ForeColor = Properties.Settings.Default.ForeColor;
            LoadConf.ForeColor = Properties.Settings.Default.ForeColor;
            SaveConf.ForeColor = Properties.Settings.Default.ForeColor;
            SaveNewConf.ForeColor = Properties.Settings.Default.ForeColor;

            AimTimeText.ForeColor = Properties.Settings.Default.ForeColor;
            AimTimeNum.ForeColor = Properties.Settings.Default.ForeColor;
            OveraimText.ForeColor = Properties.Settings.Default.ForeColor;
            OveraimFactor.ForeColor = Properties.Settings.Default.ForeColor;
            AntiTransfer.ForeColor = Properties.Settings.Default.ForeColor;
            AntitransferText.ForeColor = Properties.Settings.Default.ForeColor;
            FOV.ForeColor = Properties.Settings.Default.ForeColor;
            RangeDec.ForeColor = Properties.Settings.Default.ForeColor;
            FovText.ForeColor = Properties.Settings.Default.ForeColor;
            RangeDecFactor.ForeColor = Properties.Settings.Default.ForeColor;
            FovChart.ForeColor = Properties.Settings.Default.ForeColor;
            BoneID.ForeColor = Properties.Settings.Default.ForeColor;
            RecoilControlText.ForeColor = Properties.Settings.Default.ForeColor;
            output.ForeColor = Properties.Settings.Default.ForeColor;
            output.BackColor = Properties.Settings.Default.BackColorTwo;
            panel2.BackColor = Properties.Settings.Default.BackColorTwo;
            BoneList.ForeColor = Properties.Settings.Default.ForeColor;

            FovChart.ChartAreas[0].AxisX.LabelStyle.ForeColor = Properties.Settings.Default.ForeColor;
            FovChart.ChartAreas[0].AxisX.TitleForeColor = Properties.Settings.Default.ForeColor;
            FovChart.ChartAreas[0].AxisX.LineColor = Properties.Settings.Default.ForeColor;
            FovChart.ChartAreas[0].AxisY.LabelStyle.ForeColor = Properties.Settings.Default.ForeColor;
            FovChart.ChartAreas[0].AxisY.TitleForeColor = Properties.Settings.Default.ForeColor;
            FovChart.ChartAreas[0].AxisY.LineColor = Properties.Settings.Default.ForeColor;
            FovChart.PaletteCustomColors[0] = Properties.Settings.Default.ForeColor;

        }
        private void Form1_FormClosing(Object sender, FormClosingEventArgs e)
        {
            currentSettings.bhopOn = false;
            currentSettings.running = false;
            updateSettings(ref currentSettings);
        }

        public void outputSuccess(string text)
        {
            output.ForeColor = Success;
            output.Text = text;
        }

        public void outputFailure(string text)
        {
            output.ForeColor = Error;
            output.Text = text;
        }
        //Refreshes displayed values of all options
        void refreshValues()
        {
            makeGraph();
            AimbotButton.Text = "Aimbot | " + (currentSettings.aimOn ? "ON" : "OFF");
            RecoilControlText.Text = "Recoil Control Percentage : " + currentSettings.recoilControlFactor * 50 + "%";
            RadarButton.Text = "Radar | " + onOff(currentSettings.radar);
            FOV.Value = (decimal)currentSettings.fov;
            RangeBar.Value = (int)(currentSettings.rangeDecMult * 10);
            Thread bhopThread = new Thread(new ThreadStart(doBhop));
            bhopThread.Start();
            BHopButton.Text = "BHOP | " + onOff(currentSettings.bhopOn);
            TeamWallButton.Text = "Team ESP | " + onOff(currentSettings.teamESP);
            string display = "";
            if (currentSettings.enemyESP == 0)
            {
                display = "OFF";
            }
            else if (currentSettings.enemyESP == 1)
            {
                display = "VISIBLE";
            }
            else
            {
                display = "ON";
            }

            EnemyWallButton.Text = "Enemy ESP | " + display;
            AntiFlashButton.Text = "Anti-flash | " + onOff(currentSettings.antiFlash);
            TriggerButton.Text = "Trigger Bot | " + onOff(currentSettings.triggerOn);
            AimTimeNum.Value = (decimal)currentSettings.aimTime;
            AimTimeBar.Value = (int)(currentSettings.aimTime);
            OveraimButton.Text = "Overaim | " + onOff(currentSettings.overaim);
            SilentAimButton.Text = "Silent Aim | " + onOff(currentSettings.silentAim);
        }

        private void hidePanels()
        {
            AimPanel.Visible = false;
            AimPanel.Dock = DockStyle.Fill;
            WallsPanel.Visible = false;
            WallsPanel.Dock = DockStyle.Fill;
            MiscPanel.Visible = false;
            MiscPanel.Dock = DockStyle.Fill;
            ConfigPanel.Visible = false;
            ConfigPanel.Dock = DockStyle.Fill;
        }
        //Creates the FOV graph
        private void makeGraph()
        {
            FovChart.Series[0].Points.Clear();
            for (int i = 0; i < 50; i++)
            {
                FovChart.Series[0].Points.AddXY(i * 50, currentSettings.fov / (currentSettings.rangeDecMult*i/10+1));
            }
        }

        private void AimMenu_Click(object sender, EventArgs e)
        {
            hidePanels();
            AimPanel.Visible = true;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            hidePanels();
            MiscPanel.Visible = true;
        }

        private void WallMenu_Click(object sender, EventArgs e)
        {
            hidePanels();
            WallsPanel.Visible = true;
        }

        private void RecoilControl_Scroll(object sender, EventArgs e)
        {
            RecoilControlText.Text = "Recoil Control Percentage : " + RecoilControl.Value + "%";
            currentSettings.recoilControlFactor = RecoilControl.Value / 50f;
            settingsChanged = true;

        }

        private void RadarButton_Click(object sender, EventArgs e)
        {
            currentSettings.radar = !currentSettings.radar;
            RadarButton.Text = "Radar | " + onOff(currentSettings.radar);
            settingsChanged= true;
        }

        private void button1_Click_1(object sender, EventArgs e)
        {
            hidePanels();
            ConfigPanel.Visible = true;
        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {

            currentSettings.fov = trackBar1.Value / 10;
            FOV.Value = (decimal)currentSettings.fov;
            makeGraph();
            settingsChanged= true;
        }


        private void FOV_ValueChanged(object sender, EventArgs e)
        {
            currentSettings.fov = (float)FOV.Value;
            trackBar1.Value = (int)(currentSettings.fov * 10);
            makeGraph();
            settingsChanged= true;
        }


        private void Range_ValueChanged(object sender, EventArgs e)
        {
            currentSettings.rangeDecMult = (float)RangeDec.Value;
            RangeBar.Value = (int)(currentSettings.rangeDecMult * 10);
            makeGraph();
            settingsChanged= true;
        }

        private void RangeBar_Scroll(object sender, EventArgs e)
        {
            currentSettings.rangeDecMult = RangeBar.Value/10;
            RangeDec.Value = (decimal)currentSettings.rangeDecMult;
            makeGraph();
            settingsChanged= true;
        }


        private void BHopButton_Click(object sender, EventArgs e)
        {
            currentSettings.bhopOn = !currentSettings.bhopOn;
            settingsChanged=true;
            Thread bhopThread = new Thread(new ThreadStart(doBhop));
            bhopThread.Start();
            BHopButton.Text = "BHOP | " + onOff(currentSettings.bhopOn);

        }

        private void Refresh_Click(object sender, EventArgs e)
        {
            //kill Threads
            currentSettings.running = false;
            Thread.Sleep(1000);
            currentSettings.running = true;
            start(this);
        }

        private void BoneList_SelectedIndexChanged(object sender, EventArgs e)
        {

            
            currentSettings.boneId = Bones[BoneList.Text];
            settingsChanged=true;
        }

        private void AimbotButton_Click(object sender, EventArgs e)
        {
            currentSettings.aimOn = !currentSettings.aimOn;
            AimbotButton.Text = "Aimbot | " + onOff(currentSettings.aimOn);
            settingsChanged=true;
        }

        private void TeamESP_click(object sender, EventArgs e)
        {
            currentSettings.teamESP = !currentSettings.teamESP;
            TeamWallButton.Text = "Team ESP | " + onOff(currentSettings.teamESP);
            settingsChanged=true;
        }

        private void EnemyWallButton_Click(object sender, EventArgs e)
        {
            currentSettings.enemyESP += 1;
            currentSettings.enemyESP %= 3;
            string display = "";
            if (currentSettings.enemyESP == 0)
            {
                display = "OFF";
            }else if (currentSettings.enemyESP == 1)
            {
                display = "VISIBLE";
            }
            else
            {
                display = "ON";
            }

            EnemyWallButton.Text = "Enemy ESP | " + display;
            settingsChanged=true;
        }

        private void AntiFlashButton_Click(object sender, EventArgs e)
        {
            currentSettings.antiFlash = !currentSettings.antiFlash;
            AntiFlashButton.Text = "Anti-flash | " + onOff(currentSettings.antiFlash);
            settingsChanged=true;
        }

        private void TriggerButton_Click(object sender, EventArgs e)
        {
            currentSettings.triggerOn = !currentSettings.triggerOn;
            TriggerButton.Text = "Trigger Bot | " + onOff(currentSettings.triggerOn);
            settingsChanged=true;
        }

        

        private void TeamColorButton_Click(object sender, EventArgs e)
        {
            TeamColor.ShowDialog();
            currentSettings.friendlyR = TeamColor.Color.R;
            currentSettings.friendlyG = TeamColor.Color.G;
            currentSettings.friendlyB = TeamColor.Color.B;
            settingsChanged=true;
        }

        private void EnemyColorButton_click(object sender, EventArgs e)
        {
            EnemyColor.ShowDialog();
            currentSettings.enemyR = EnemyColor.Color.R;
            currentSettings.enemyG = EnemyColor.Color.G;
            currentSettings.enemyB = EnemyColor.Color.B;
            settingsChanged=true;
        }

        private void DriverInject_Click(object sender, EventArgs e)
        {
            
        }

        private void LoadConf_Click(object sender, EventArgs e)
        {
            OpenConfigDialog.InitialDirectory = AppDomain.CurrentDomain.BaseDirectory;
            if (OpenConfigDialog.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    loadConfig(OpenConfigDialog.FileName);
                    refreshValues();
                    Properties.Settings.Default.ConfigPath = OpenConfigDialog.FileName;
                    Properties.Settings.Default.Save();

                    output.ForeColor = Success;
                    output.Text = "Successfully Opened Config '" + System.IO.Path.GetFileName(OpenConfigDialog.FileName) + "'"; ;
                }
                catch 
                {
                    SystemSounds.Exclamation.Play();
                    output.ForeColor = Error;
                    output.Text = "Failed To Load Config";

                }
            }
            settingsChanged=true;
        }

        private void SaveConf_Click(object sender, EventArgs e)
        {
            try
            {
                saveConfig();

                output.ForeColor = Success;
                output.Text = "Successfully Saved Config";
            }
            catch
            {
                SystemSounds.Exclamation.Play();
                output.ForeColor = Error;
                output.Text = "Failed To Save Config";

            }
        }

        private void SaveNewConf_Click(object sender, EventArgs e)
        {
            SaveConfigDialog.InitialDirectory = AppDomain.CurrentDomain.BaseDirectory;
            if (SaveConfigDialog.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    saveConfig(SaveConfigDialog.FileName);

                    output.ForeColor = Success;
                    output.Text = "Successfully Saved Config '" + System.IO.Path.GetFileName(SaveConfigDialog.FileName) + "'";
                }
                catch
                {
                    SystemSounds.Exclamation.Play();
                    output.ForeColor = Error;
                    output.Text = "Failed To Save Config";

                }
            }
        }

        private void AimTimeBar_Scroll(object sender, EventArgs e)
        {
           

            currentSettings.aimTime = (float)AimTimeBar.Value;
            AimTimeNum.Value = (decimal)currentSettings.aimTime;
            settingsChanged=true;
        }
        private void AimTimeNum_ValueChanged(object sender, EventArgs e)
        {
 
            currentSettings.aimTime = (float)AimTimeNum.Value;
            AimTimeBar.Value = (int)currentSettings.aimTime;
            settingsChanged=true;
        }


        private void OveraimFactor_ValueChanged(object sender, EventArgs e)
        {
            currentSettings.overAimFactor = (float)OveraimFactor.Value;
            settingsChanged=true;
        }

        private void AntiTransfer_ValueChanged(object sender, EventArgs e)
        {
            currentSettings.antiTransfer = (float)AntiTransfer.Value / 1000;
            settingsChanged=true;
        }

        private void OveraimButton_Click(object sender, EventArgs e)
        {
            currentSettings.overaim = !currentSettings.overaim;
            OveraimButton.Text = "Overaim | " + onOff(currentSettings.overaim);
            settingsChanged=true;
        }

        private void SilentAimButton_Click(object sender, EventArgs e)
        {
            currentSettings.silentAim = !currentSettings.silentAim;
            SilentAimButton.Text = "Silent Aim | " + onOff(currentSettings.silentAim);
            settingsChanged=true;
        }

        private void StartButton_Click(object sender, EventArgs e)
        {
            start(this);
        }
    }
}
