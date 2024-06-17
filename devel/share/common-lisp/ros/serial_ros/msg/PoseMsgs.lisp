; Auto-generated. Do not edit!


(cl:in-package serial_ros-msg)


;//! \htmlinclude PoseMsgs.msg.html

(cl:defclass <PoseMsgs> (roslisp-msg-protocol:ros-message)
  ((poseMsgs
    :reader poseMsgs
    :initarg :poseMsgs
    :type (cl:vector geometry_msgs-msg:PoseWithCovarianceStamped)
   :initform (cl:make-array 0 :element-type 'geometry_msgs-msg:PoseWithCovarianceStamped :initial-element (cl:make-instance 'geometry_msgs-msg:PoseWithCovarianceStamped)))
   (carId
    :reader carId
    :initarg :carId
    :type cl:string
    :initform ""))
)

(cl:defclass PoseMsgs (<PoseMsgs>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <PoseMsgs>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'PoseMsgs)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name serial_ros-msg:<PoseMsgs> is deprecated: use serial_ros-msg:PoseMsgs instead.")))

(cl:ensure-generic-function 'poseMsgs-val :lambda-list '(m))
(cl:defmethod poseMsgs-val ((m <PoseMsgs>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader serial_ros-msg:poseMsgs-val is deprecated.  Use serial_ros-msg:poseMsgs instead.")
  (poseMsgs m))

(cl:ensure-generic-function 'carId-val :lambda-list '(m))
(cl:defmethod carId-val ((m <PoseMsgs>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader serial_ros-msg:carId-val is deprecated.  Use serial_ros-msg:carId instead.")
  (carId m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <PoseMsgs>) ostream)
  "Serializes a message object of type '<PoseMsgs>"
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'poseMsgs))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'poseMsgs))
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'carId))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'carId))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <PoseMsgs>) istream)
  "Deserializes a message object of type '<PoseMsgs>"
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'poseMsgs) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'poseMsgs)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'geometry_msgs-msg:PoseWithCovarianceStamped))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'carId) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'carId) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<PoseMsgs>)))
  "Returns string type for a message object of type '<PoseMsgs>"
  "serial_ros/PoseMsgs")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'PoseMsgs)))
  "Returns string type for a message object of type 'PoseMsgs"
  "serial_ros/PoseMsgs")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<PoseMsgs>)))
  "Returns md5sum for a message object of type '<PoseMsgs>"
  "8c76c3222856d20930581a2a503a432d")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'PoseMsgs)))
  "Returns md5sum for a message object of type 'PoseMsgs"
  "8c76c3222856d20930581a2a503a432d")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<PoseMsgs>)))
  "Returns full string definition for message of type '<PoseMsgs>"
  (cl:format cl:nil "geometry_msgs/PoseWithCovarianceStamped[] poseMsgs~%string carId~%================================================================================~%MSG: geometry_msgs/PoseWithCovarianceStamped~%# This expresses an estimated pose with a reference coordinate frame and timestamp~%~%Header header~%PoseWithCovariance pose~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%================================================================================~%MSG: geometry_msgs/PoseWithCovariance~%# This represents a pose in free space with uncertainty.~%~%Pose pose~%~%# Row-major representation of the 6x6 covariance matrix~%# The orientation parameters use a fixed-axis representation.~%# In order, the parameters are:~%# (x, y, z, rotation about X axis, rotation about Y axis, rotation about Z axis)~%float64[36] covariance~%~%================================================================================~%MSG: geometry_msgs/Pose~%# A representation of pose in free space, composed of position and orientation. ~%Point position~%Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'PoseMsgs)))
  "Returns full string definition for message of type 'PoseMsgs"
  (cl:format cl:nil "geometry_msgs/PoseWithCovarianceStamped[] poseMsgs~%string carId~%================================================================================~%MSG: geometry_msgs/PoseWithCovarianceStamped~%# This expresses an estimated pose with a reference coordinate frame and timestamp~%~%Header header~%PoseWithCovariance pose~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%================================================================================~%MSG: geometry_msgs/PoseWithCovariance~%# This represents a pose in free space with uncertainty.~%~%Pose pose~%~%# Row-major representation of the 6x6 covariance matrix~%# The orientation parameters use a fixed-axis representation.~%# In order, the parameters are:~%# (x, y, z, rotation about X axis, rotation about Y axis, rotation about Z axis)~%float64[36] covariance~%~%================================================================================~%MSG: geometry_msgs/Pose~%# A representation of pose in free space, composed of position and orientation. ~%Point position~%Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <PoseMsgs>))
  (cl:+ 0
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'poseMsgs) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
     4 (cl:length (cl:slot-value msg 'carId))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <PoseMsgs>))
  "Converts a ROS message object to a list"
  (cl:list 'PoseMsgs
    (cl:cons ':poseMsgs (poseMsgs msg))
    (cl:cons ':carId (carId msg))
))
