#! /usr/bin/env python
import sys
import roslib; 
import rospy
import actionlib
import time
from control_msgs.msg import *
from trajectory_msgs.msg import *
from prx_planning.srv import *
from unigripper import *


ROBOT_AC_SERVER_TIMEOUT = 5
SIM_STEP = 0.1
motoman_joint_names = ["torso_joint_b1", "arm_left_joint_1_s", "arm_left_joint_2_l", "arm_left_joint_3_e", "arm_left_joint_4_u", "arm_left_joint_5_r", "arm_left_joint_6_b", "arm_left_joint_7_t", "arm_right_joint_1_s", "arm_right_joint_2_l", "arm_right_joint_3_e", "arm_right_joint_4_u", "arm_right_joint_5_r", "arm_right_joint_6_b", "arm_right_joint_7_t",  "torso_joint_b2"]
unigripper_joint_names = ["head_hinge"]

motoman_command = FollowJointTrajectoryGoal()
unigripper_command = FollowJointTrajectoryGoal()

for name in motoman_joint_names[1:-1]:
  motoman_command.trajectory.joint_names.append(name)
motoman_command.trajectory.joint_names.append(motoman_joint_names[0])
motoman_command.trajectory.joint_names.append(motoman_joint_names[-1])

for name in unigripper_joint_names:
  unigripper_command.trajectory.joint_names.append(name)


# print motoman_command.trajectory.joint_names
# print unigripper_command.trajectory.joint_names


ac = actionlib.SimpleActionClient('joint_trajectory_action', FollowJointTrajectoryAction) 
ac_uni = actionlib.SimpleActionClient('unigripper/unigripper_joint_trajectory_action', FollowJointTrajectoryAction)


gripper = UniGripper()







sample_traj=[
[0.00000,1.57000,0.00000,0.00000,-1.70000,-0.00002,0.00000,0.00000,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
,[-0.05556,1.54808,0.01163,0.07080,-1.68671,-0.00477,0.00284,0.02362,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
,[-0.11113,1.52617,0.02325,0.14160,-1.67342,-0.00952,0.00569,0.04723,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
,[-0.16669,1.50425,0.03488,0.21240,-1.66013,-0.01427,0.00853,0.07085,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
,[-0.22226,1.48234,0.04651,0.28320,-1.64684,-0.01903,0.01138,0.09446,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
,[-0.27782,1.46042,0.05813,0.35400,-1.63355,-0.02378,0.01422,0.11808,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
,[-0.33338,1.43851,0.06976,0.42481,-1.62026,-0.02853,0.01707,0.14169,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
,[-0.38895,1.41660,0.08139,0.49561,-1.60697,-0.03328,0.01991,0.16531,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
,[-0.44451,1.39468,0.09301,0.56641,-1.59368,-0.03804,0.02276,0.18892,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
,[-0.50007,1.37277,0.10464,0.63721,-1.58039,-0.04279,0.02560,0.21254,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
,[-0.55564,1.35085,0.11627,0.70801,-1.56710,-0.04754,0.02844,0.23615,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
,[-0.61120,1.32894,0.12789,0.77881,-1.55381,-0.05229,0.03129,0.25977,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
,[-0.66677,1.30702,0.13952,0.84961,-1.54052,-0.05705,0.03413,0.28338,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
,[-0.72233,1.28511,0.15114,0.92041,-1.52723,-0.06180,0.03698,0.30700,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
,[-0.77789,1.26320,0.16277,0.99121,-1.51394,-0.06655,0.03982,0.33062,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]]
# ,[-0.83346,1.24128,0.17440,1.06201,-1.50065,-0.07130,0.04267,0.35423,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.81957,1.20336,0.17206,0.98894,-1.53672,-0.01828,0.10486,0.30056,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.80569,1.16543,0.16973,0.91587,-1.57279,0.03473,0.16705,0.24688,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.79181,1.12751,0.16740,0.84280,-1.60886,0.08775,0.22923,0.19321,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.77792,1.08959,0.16506,0.76974,-1.64493,0.14077,0.29142,0.13953,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.76404,1.05166,0.16273,0.69667,-1.68099,0.19379,0.35361,0.08586,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.75015,1.01374,0.16040,0.62360,-1.71706,0.24681,0.41580,0.03219,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.73627,0.97581,0.15806,0.55053,-1.75313,0.29983,0.47799,-0.02149,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.72239,0.93789,0.15573,0.47746,-1.78920,0.35285,0.54018,-0.07516,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.70850,0.89997,0.15339,0.40439,-1.82527,0.40587,0.60237,-0.12883,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.69462,0.86204,0.15106,0.33132,-1.86134,0.45889,0.66456,-0.18251,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.68074,0.82412,0.14873,0.25825,-1.89741,0.51190,0.72675,-0.23618,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.66685,0.78619,0.14639,0.18518,-1.93348,0.56492,0.78894,-0.28986,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.65297,0.74827,0.14406,0.11211,-1.96955,0.61794,0.85113,-0.34353,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.63908,0.71035,0.14173,0.03904,-2.00561,0.67096,0.91332,-0.39720,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.62520,0.67242,0.13939,-0.03403,-2.04168,0.72398,0.97551,-0.45088,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.61132,0.63450,0.13706,-0.10710,-2.07775,0.77700,1.03770,-0.50455,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.59743,0.59658,0.13472,-0.18017,-2.11382,0.83002,1.09989,-0.55823,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.58355,0.55865,0.13239,-0.25324,-2.14989,0.88304,1.16208,-0.61190,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.56966,0.52073,0.13006,-0.32631,-2.18596,0.93606,1.22427,-0.66557,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.54741,0.50412,0.12036,-0.33887,-2.23407,0.99799,1.25196,-0.72928,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.52515,0.48752,0.11067,-0.35144,-2.28217,1.05992,1.27965,-0.79299,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.50289,0.47091,0.10098,-0.36400,-2.33028,1.12185,1.30734,-0.85669,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.47516,0.39902,0.13276,-0.33442,-2.33008,1.10542,1.31072,-0.81160,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.44744,0.32714,0.16454,-0.30484,-2.32988,1.08900,1.31409,-0.76651,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.41971,0.25525,0.19631,-0.27525,-2.32968,1.07257,1.31747,-0.72142,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.39199,0.18336,0.22809,-0.24567,-2.32947,1.05614,1.32085,-0.67634,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.36426,0.11147,0.25987,-0.21609,-2.32927,1.03972,1.32422,-0.63125,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.33653,0.03959,0.29165,-0.18651,-2.32907,1.02329,1.32760,-0.58616,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.30881,-0.03230,0.32343,-0.15693,-2.32887,1.00686,1.33098,-0.54107,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.28108,-0.10419,0.35521,-0.12735,-2.32867,0.99044,1.33435,-0.49598,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.25335,-0.17608,0.38699,-0.09777,-2.32847,0.97401,1.33773,-0.45089,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.22563,-0.24797,0.41876,-0.06819,-2.32826,0.95758,1.34111,-0.40580,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.19790,-0.31985,0.45054,-0.03861,-2.32806,0.94116,1.34448,-0.36071,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.17018,-0.39174,0.48232,-0.00903,-2.32786,0.92473,1.34786,-0.31562,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.14245,-0.46363,0.51410,0.02056,-2.32766,0.90830,1.35124,-0.27053,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.11472,-0.53552,0.54588,0.05014,-2.32746,0.89187,1.35461,-0.22545,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.08700,-0.60740,0.57766,0.07972,-2.32726,0.87545,1.35799,-0.18036,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.05927,-0.67929,0.60944,0.10930,-2.32705,0.85902,1.36137,-0.13527,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.03154,-0.75118,0.64121,0.13888,-2.32685,0.84259,1.36474,-0.09018,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[-0.00382,-0.82307,0.67299,0.16846,-2.32665,0.82617,1.36812,-0.04509,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ,[0.02391,-0.89496,0.70477,0.19804,-2.32645,0.80974,1.37150,0.00000,1.57000,0.00000,0.00000,-1.70000,0.00000,0.00000,0.00000,0.00000,0.00000]
# ]







reversed_traj = []
for state in sample_traj:
  reversed_traj = [state] + reversed_traj

trajectory_total = []
trajectory_chunk = []
chunk_id = 0
gripper_toggle = 0






def parse_file ( filename ) :
  global trajectory_total, trajectory_chunk, gripper_toggle, chunk_id
  fp = open(filename,"r")
  print "Parsing trajectory file " , filename

  for line in fp.readlines():
    float_line = [float(val) for val in line.split(',')]
    new_gripper_toggle = int(float_line[-1])
    if(gripper_toggle != new_gripper_toggle):
      trajectory_total.append(trajectory_chunk)
      gripper_toggle = new_gripper_toggle
      trajectory_chunk = []
      chunk_id = chunk_id + 1
      print "MODE CHANGE"
    trajectory_chunk.append(float_line)
    print float_line

  print len(trajectory_total)
  print chunk_id





def communicate_trajectory(traj, speed):
  
  motoman_command.trajectory.points = []
  unigripper_command.trajectory.points = []

  print "Starting to wait for server"
  ac.wait_for_server(rospy.Duration(5))
  print "Done waiting for server"

  print "Communicating trajectory of size: " , len(traj) , " Speed: " ,speed

  duration = 0;
  if len(traj) == 0:
      return True
  
  #Get from pracsys
  sim_step = SIM_STEP
  if len(traj)==2:
      sim_step = .5

  for state in traj:
    point = JointTrajectoryPoint()
    point.time_from_start = rospy.Duration(duration)
    point_uni = JointTrajectoryPoint()
    point_uni.time_from_start = rospy.Duration(duration)


    for i in xrange(1,15):
      point.positions.append(state[i])
      point.velocities.append(0)
      point.accelerations.append(0)


    point.positions.append(state[0])
    point.velocities.append(0)
    point.accelerations.append(0)
    point.positions.append(state[0])
    point.velocities.append(0)
    point.accelerations.append(0)


    point_uni.positions.append(state[15])
    point_uni.velocities.append(0)
    point_uni.accelerations.append(0)

    duration += ((int)(1.0 / speed)) * sim_step
    motoman_command.trajectory.points.append(point)
    unigripper_command.trajectory.points.append(point_uni)
  



  duration_of_timeout = (len(traj)*((int)(1.0 / speed)) * sim_step)*1.5+1.5
  motoman_command.trajectory.header.stamp = rospy.Time.now()
  unigripper_command.trajectory.header.stamp = rospy.Time.now()
  
  found_ac_server = False
  while not found_ac_server:
      found_ac_server = ac.wait_for_server(rospy.Duration(ROBOT_AC_SERVER_TIMEOUT)) and ac_uni.wait_for_server(rospy.Duration(ROBOT_AC_SERVER_TIMEOUT))
  
  time.sleep(2)

  ac.send_goal(motoman_command)
  ac_uni.send_goal(unigripper_command)

  finished_before_timeout = ac.wait_for_result(rospy.Duration(duration_of_timeout))

  print "Done waiting"
  enabled_robot = True

  status = ac.get_state()
  print status

  
  if not finished_before_timeout:
    return False
  elif state == "SUCCEEDED":
    return True
  else:
    return False


def control_gripper(chunk):
  if(chunk[0][-1]==0):
    gripper.turn_off()
  else:
    if(chunk[0][-1]==1):
      gripper.turn_on()

def execute_chunks():
  gripper.turn_off()
  for chunk in trajectory_total:
    control_gripper(chunk)
    communicate_trajectory(chunk,0.2)
  gripper.turn_off()





if __name__ == '__main__':
  rospy.init_node("motoman_trajectory_execution")

  if(len(sys.argv)<2):
    print "Filename required for playback"
    # gripper.turn_on()
    gripper.turn_off()
    print "trajectory"
    print sample_traj
    communicate_trajectory(sample_traj,0.2)
    gripper.turn_on()
    # gripper.turn_off()
    print "reverse trajectory"
    print reversed(sample_traj)
    communicate_trajectory(reversed_traj,0.2)
    # gripper.turn_on()
    gripper.turn_off()
  else:
    parse_file(sys.argv[1])
    execute_chunks()



