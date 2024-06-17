// Auto-generated. Do not edit!

// (in-package serial_ros.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let geometry_msgs = _finder('geometry_msgs');

//-----------------------------------------------------------

class PoseMsgs {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.poseMsgs = null;
      this.carId = null;
    }
    else {
      if (initObj.hasOwnProperty('poseMsgs')) {
        this.poseMsgs = initObj.poseMsgs
      }
      else {
        this.poseMsgs = [];
      }
      if (initObj.hasOwnProperty('carId')) {
        this.carId = initObj.carId
      }
      else {
        this.carId = '';
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type PoseMsgs
    // Serialize message field [poseMsgs]
    // Serialize the length for message field [poseMsgs]
    bufferOffset = _serializer.uint32(obj.poseMsgs.length, buffer, bufferOffset);
    obj.poseMsgs.forEach((val) => {
      bufferOffset = geometry_msgs.msg.PoseWithCovarianceStamped.serialize(val, buffer, bufferOffset);
    });
    // Serialize message field [carId]
    bufferOffset = _serializer.string(obj.carId, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type PoseMsgs
    let len;
    let data = new PoseMsgs(null);
    // Deserialize message field [poseMsgs]
    // Deserialize array length for message field [poseMsgs]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.poseMsgs = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.poseMsgs[i] = geometry_msgs.msg.PoseWithCovarianceStamped.deserialize(buffer, bufferOffset)
    }
    // Deserialize message field [carId]
    data.carId = _deserializer.string(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    object.poseMsgs.forEach((val) => {
      length += geometry_msgs.msg.PoseWithCovarianceStamped.getMessageSize(val);
    });
    length += _getByteLength(object.carId);
    return length + 8;
  }

  static datatype() {
    // Returns string type for a message object
    return 'serial_ros/PoseMsgs';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '8c76c3222856d20930581a2a503a432d';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    geometry_msgs/PoseWithCovarianceStamped[] poseMsgs
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
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new PoseMsgs(null);
    if (msg.poseMsgs !== undefined) {
      resolved.poseMsgs = new Array(msg.poseMsgs.length);
      for (let i = 0; i < resolved.poseMsgs.length; ++i) {
        resolved.poseMsgs[i] = geometry_msgs.msg.PoseWithCovarianceStamped.Resolve(msg.poseMsgs[i]);
      }
    }
    else {
      resolved.poseMsgs = []
    }

    if (msg.carId !== undefined) {
      resolved.carId = msg.carId;
    }
    else {
      resolved.carId = ''
    }

    return resolved;
    }
};

module.exports = PoseMsgs;
