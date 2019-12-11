using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FaceWashingSpawn : MonoBehaviour
{
    private bool isActive = false;
    private Animator animator;
    private float targetSpeed;

    private float maxSpeed = 3;
    private float decayAmount = 0.02f;

    
    private Quaternion targetRotation;

    private Quaternion activateRotation = Quaternion.AngleAxis(90, Vector3.left);
    private Quaternion deactivateRotation = Quaternion.AngleAxis(90, Vector3.right);

    public int playerID = -1;
    public GameController gameController;

    public Transform parentTransform;

    private System.DateTime lastClickedOn = new System.DateTime(2019, 11, 11);

    private int completionCount = 0;

    public CountdownTimer timer;

    public ProgressTracker progressTracker;

    private bool hasCompleted = false;
    private AudioSource soundEffect;


    void Start()
    {
        animator = GetComponent<Animator>();
        targetSpeed = animator.speed = 0;
        targetSpeed = 0;
        targetRotation = parentTransform.rotation;
        soundEffect = GetComponent<AudioSource>();
    }

    void Update()
    {
        if(hasCompleted){
            soundEffect.Stop();
            animator.speed = 0;
            return;
        }

        if(animator.speed != targetSpeed){
            animator.speed = Mathf.Lerp(animator.speed, targetSpeed, 0.1f);
        }

        if(animator.speed == 0) return;

        if(animator.speed - decayAmount > 0){
            animator.speed -= decayAmount;
            
            if(!soundEffect.isPlaying){
                soundEffect.Play();
            }
        }
        else {
            animator.speed = 0;
            soundEffect.Stop();
        }
        
        if(targetSpeed - decayAmount > 0){
            targetSpeed -= decayAmount;
        }
        else{
            targetSpeed = 0;
        }

    }

    public IEnumerator Show()
    {
        isActive = true;
        targetRotation *=  activateRotation;
        float progress = 0;
        float smoothness = 0.02f;
        float duration = 1;
        float increment = smoothness/duration;
        while(progress < 1)
        {
            transform.parent.parent.rotation = Quaternion.Lerp (transform.parent.parent.rotation, targetRotation , progress); 
            progress += increment;
            yield return new WaitForSeconds(smoothness);
        }
    }

    public IEnumerator Hide(bool toDestroy)
    {
        isActive = false;
        targetRotation *=  deactivateRotation;
        float progress = 0;
        float smoothness = 0.02f;
        float duration = 1;
        float increment = smoothness/duration;
        while(progress < 1)
        {
            parentTransform.rotation = Quaternion.Lerp (parentTransform.rotation, targetRotation , progress); 
            progress += increment;
            yield return new WaitForSeconds(smoothness);
        }
        if(toDestroy){
            Destroy(transform.parent.parent.gameObject);
        }
    }

    public void Activate(){
        if(isActive) return;
        isActive = true;
        timer.StartCountdown();
        StartCoroutine(Show());
    }

    public void Deactivate(){
        if(!isActive) return;
        isActive = false;
        StartCoroutine(Hide(true));
    }

    public void Animate(float speed){
        if(hasCompleted) return;
        if(speed == -1){
            speed = 0;
        }

        else if(speed > maxSpeed){
            speed = maxSpeed;
        }

        targetSpeed = speed;
    }

    public void OnClick(char data){
        if(hasCompleted) return;
        float intensity = float.Parse(data + "");
        targetSpeed = intensity / 9f * maxSpeed;

        // if(data == '1'){
        //     System.DateTime now = System.DateTime.Now;
        //     System.TimeSpan span = now - lastClickedOn;

        //     if(span.TotalMilliseconds < 3000){
        //         targetSpeed += 3f;
        //     }
        //     else if(span.TotalMilliseconds < 6000){
        //         targetSpeed += 2f;
        //     }
        //     else if(span.TotalMilliseconds < 9000) {
        //         targetSpeed += 1f;
        //     }
        //     else{
        //         targetSpeed = 1f;
        //     }

        //     if(targetSpeed > maxSpeed){
        //         targetSpeed = maxSpeed;
        //     }

        //     lastClickedOn = now;
        // }
    }

    public void AnimationEnded(){
        completionCount++;
        if(completionCount == 3 || completionCount == 6 || completionCount == 9 || completionCount == 12){
            progressTracker.Increment(playerID);
        }
        else if(completionCount == 15){
            hasCompleted = true;
            progressTracker.Increment(playerID);
            gameController.missionComplete(playerID, transform.parent.parent.rotation);
            iTween.ScaleTo(gameObject, iTween.Hash("scale", Vector3.zero, "time", 3));
        }
    }

    public GameObject GetGameObject(){
        return gameObject;
    }

}
