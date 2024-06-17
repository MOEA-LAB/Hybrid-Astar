
(cl:in-package :asdf)

(defsystem "serial_ros-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :geometry_msgs-msg
)
  :components ((:file "_package")
    (:file "PoseMsgs" :depends-on ("_package_PoseMsgs"))
    (:file "_package_PoseMsgs" :depends-on ("_package"))
  ))