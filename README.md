# HW3_NETWORKS
CC algorithms 

Tcp Congestion:-
Transmission Control Protocol (TCP) uses a network congestion-avoidance algorithm that includes various aspects of an additive increase/multiplicative decrease (AIMD) scheme,
along with other schemes including slow start[1] and congestion window (CWND), to achieve congestion avoidance. The TCP congestion-avoidance algorithm is the primary basis for congestion control in the Internet.[2][3][4] Per the end-to-end principle,
congestion control is largely a function of internet hosts, not the network itself. There are several variations and versions of the algorithm implemented in protocol stacks of operating systems of computers that connect to the Internet.
Congestion window
In TCP, the congestion window (CWND) is one of the factors that determines the number of bytes that can be sent out at any time. The congestion window is maintained by the sender and is a means of stopping a link between the sender and the receiver from becoming overloaded with too much traffic. This should not be confused with the sliding window maintained by the sender which exists to prevent the receiver from becoming overloaded. The congestion window is calculated by estimating how much congestion there is on the link.
When a connection is set up, the congestion window, a value maintained independently at each host, is set to a small multiple of the maximum segment size (MSS) allowed on that connection. Further variance in the congestion window is dictated by an additive increase/multiplicative decrease (AIMD) approach. This means that if all segments are received and the acknowledgments reach the sender on time, some constant is added to the window size. It will follow different algorithms.
A system administrator may adjust the maximum window size limit, or adjust the constant added during additive increase, as part of TCP tuning.
The flow of data over a TCP connection is also controlled by the use of the receive window advertised by the receiver. A sender can send data less than its own congestion window and the receive window.
￼
<img width="785" alt="Screenshot 2022-12-15 at 14 32 03" src="https://user-images.githubusercontent.com/92846018/207860338-e26b011c-612d-4bfc-864b-3113f1b4a70c.png">

CUBIC is a network congestion avoidance algorithm for TCP which can achieve high bandwidth connections over networks more quickly and reliably in the face of high latency than earlier algorithms. It helps optimize long fat networks.
CUBIC increases its window to be real-time dependent, not RTT dependent like BIC. The calculation for cwnd (congestion window) is simpler than BIC, too.
Define the following variables:
● β: Multiplicative decrease factor
● wmax: Window size just before the last reduction
● T: Time elapsed since the last window reduction
● C: A scaling constant
● cwnd: The congestion window at the current time

<img width="785" alt="Screenshot 2022-12-15 at 14 32 11" src="https://user-images.githubusercontent.com/92846018/207860445-d8de9906-1fd1-4391-a69b-8af4976f3ac6.png">

Reno is the extension of TCP Tahoe, and NewReno is the extension of TCP Reno. In Reno, when packet loss occurs, the sender reduces the
cwnd by 50% along with the ssthresh value. This would allow the network to come out of the congestion state easily. But Reno suffered from a very critical backlog which hurts its performance.
When multiple packets are dropped in the same congestion window (say 1-10) then every time it knows about a packet loss it reduces the cwnd by 50%. So, for 2 packet loss, it will reduce the cwnd by 4 times (50% twice). But, one reduction of 50% per congestion window was enough for recovering all those lost packets. Say cwnd=1024 and 10 packets are dropped in this window, Reno will reduce cwnd by 50% 10 times, finally cwnd=1024/210 = 1, it is astonishing. It would take 10 RTTs by the sender to again grow its cwnd up to 1024 using slow start leave alone the AIMD algorithm.

<img width="852" alt="Screenshot 2022-12-15 at 14 33 11" src="https://user-images.githubusercontent.com/92846018/207860560-57c02413-de7c-48ca-9f05-8fdff4f6efea.png">


<img width="852" alt="Screenshot 2022-12-15 at 14 33 27" src="https://user-images.githubusercontent.com/92846018/207860610-5a33dd73-5b1a-44f0-a826-3ace270ebd2f.png">
