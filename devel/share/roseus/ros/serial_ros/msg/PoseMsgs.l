;; Auto-generated. Do not edit!


(when (boundp 'serial_ros::PoseMsgs)
  (if (not (find-package "SERIAL_ROS"))
    (make-package "SERIAL_ROS"))
  (shadow 'PoseMsgs (find-package "SERIAL_ROS")))
(unless (find-package "SERIAL_ROS::POSEMSGS")
  (make-package "SERIAL_ROS::POSEMSGS"))

(in-package "ROS")
;;//! \htmlinclude PoseMsgs.msg.html
(if (not (find-package "GEOMETRY_MSGS"))
  (ros::roseus-add-msgs "geometry_msgs"))


(defclass serial_ros::PoseMsgs
  :super ros::object
  :slots (_poseMsgs _carId ))

(defmethod serial_ros::PoseMsgs
  (:init
   (&key
    ((:poseMsgs __poseMsgs) ())
    ((:carId __carId) "")
    )
   (send-super :init)
   (setq _poseMsgs __poseMsgs)
   (setq _carId (string __carId))
   self)
  (:poseMsgs
   (&rest __poseMsgs)
   (if (keywordp (car __poseMsgs))
       (send* _poseMsgs __poseMsgs)
     (progn
       (if __poseMsgs (setq _poseMsgs (car __poseMsgs)))
       _poseMsgs)))
  (:carId
   (&optional __carId)
   (if __carId (setq _carId __carId)) _carId)
  (:serialization-length
   ()
   (+
    ;; geometry_msgs/PoseWithCovarianceStamped[] _poseMsgs
    (apply #'+ (send-all _poseMsgs :serialization-length)) 4
    ;; string _carId
    4 (length _carId)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; geometry_msgs/PoseWithCovarianceStamped[] _poseMsgs
     (write-long (length _poseMsgs) s)
     (dolist (elem _poseMsgs)
       (send elem :serialize s)
       )
     ;; string _carId
       (write-long (length _carId) s) (princ _carId s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; geometry_msgs/PoseWithCovarianceStamped[] _poseMsgs
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _poseMsgs (let (r) (dotimes (i n) (push (instance geometry_msgs::PoseWithCovarianceStamped :init) r)) r))
     (dolist (elem- _poseMsgs)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;; string _carId
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _carId (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;;
   self)
  )

(setf (get serial_ros::PoseMsgs :md5sum-) "8c76c3222856d20930581a2a503a432d")
(setf (get serial_ros::PoseMsgs :datatype-) "serial_ros/PoseMsgs")
(setf (get serial_ros::PoseMsgs :definition-)
      "geometry_msgs/PoseWithCovarianceStamped[] poseMsgs
string carId
================================================================================
MSG: geometry_msgs/PoseWithCovarianceStamped
# This expresses an estimated pose with a reference coordinate frame and timestamp

Header header
PoseWithCovariance pose

================================================================================
MSG: std_msgs/Header
# Standard metadata for higher-level stamped data types.
# This is generally used to communicate timestamped data 
# in a particular coordinate frame.
# 
# sequence ID: consecutively increasing ID 
uint32 seq
#Two-integer timestamp that is expressed as:
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
# time-handling sugar is provided by the client library
time stamp
#Frame this data is associated with
string frame_id

================================================================================
MSG: geometry_msgs/PoseWithCovariance
# This represents a pose in free space with uncertainty.

Pose pose

# Row-major representation of the 6x6 covariance matrix
# The orientation parameters use a fixed-axis representation.
# In order, the parameters are:
# (x, y, z, rotation about X axis, rotation about Y axis, rotation about Z axis)
float64[36] covariance

================================================================================
MSG: geometry_msgs/Pose
# A representation of pose in free space, composed of position and orientation. 
Point position
Quaternion orientation

================================================================================
MSG: geometry_msgs/Point
# This contains the position of a point in free space
float64 x
float64 y
float64 z

================================================================================
MSG: geometry_msgs/Quaternion
# This represents an orientation in free space in quaternion form.

float64 x
float64 y
float64 z
float64 w

")



(provide :serial_ros/PoseMsgs "8c76c3222856d20930581a2a503a432d")


