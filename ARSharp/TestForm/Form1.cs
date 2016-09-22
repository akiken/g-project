using MMF.Controls.Forms;
using MMF.DeviceManager;
using MMF.Grid;
using MMF.Kinect;
using MMF.Matricies.Camera.CameraMotion;
using MMF.Model.Controller;
using MMF.Model.PMX;
using SlimDX;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TestForm
{
    public partial class Form1 : D2DSupportedRenderForm
    {
        TransformController Controller;
        PMXModel Model;
        float t = 0;

        public Form1()
        {
            InitializeComponent();
        }

        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);
            WorldSpace.AddDrawableGroup(new ControllerDrawableGroup(1, "Controller", RenderContext));
            KeyPreview = true;
            Controller = new TransformController(RenderContext, ScreenContext.HitChekcer);
            WorldSpace.AddResource(Controller, "Controller");

            ScreenContext.CameraMotionProvider = new BasicCameraControllerMotionProvider(this, this);
            BasicGrid gird = new BasicGrid();
            gird.Visibility = true;
            gird.Load(RenderContext);

            ScreenContext.WorldSpace.AddResource(gird);

            Model = PMXModelWithPhysics.OpenLoad(@"..\..\..\model\初音ミク2.1Cstyle\初音ミクver.2.1Cstyle.pmx", RenderContext);
            Model.Transformer.Position = new Vector3(0, 0, 0);
            WorldSpace.AddResource(Model);
            //Form1.Controller.setTargetModel(Model);
            //Form1.Controller.Type = TransformController.TransformType.All;
        }

        protected override void OnClosing(CancelEventArgs e)
        {
            base.OnClosing(e);
            e.Cancel = false;
        }

        protected override void RenderSprite()
        {

        }

        public override void OnUpdated()
        {
            base.OnUpdated();
        }

        protected override void OnPresented()
        {
            base.OnPresented();
            RenderContext.LightManager.Position = new Vector3((float)Math.Cos(t), 0, (float)Math.Sin(t)) * 10f;
            t += 0.0001f;
        }

        
    }
}
