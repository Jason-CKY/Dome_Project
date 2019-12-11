using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ControllerRotater : MonoBehaviour
{
    public GameObject scaler;
    public GameObject pivot;
    void Start(){
        iTween.RotateAdd(pivot, iTween.Hash("amount", new Vector3(20, 0, 0), "time", 2f, 
            "looptype", iTween.LoopType.pingPong, "easetype", iTween.EaseType.easeInOutQuad));
    }

    public void Show(){
        iTween.ScaleTo(scaler, iTween.Hash("scale", Vector3.one, "time", 1f));
    }
    public void Hide(){
        iTween.ScaleTo(scaler, iTween.Hash("scale", Vector3.zero, "time", 1f));
    }

}
