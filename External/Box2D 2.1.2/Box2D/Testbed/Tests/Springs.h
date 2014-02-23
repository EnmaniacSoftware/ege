#ifndef SPRINGS_H
#define SPRINGS_H

class SpringQueryCallback : public b2QueryCallback
{
public:
	SpringQueryCallback(const b2Vec2& point)
	{
		m_point = point;
		m_fixture = NULL;
	}

	bool ReportFixture(b2Fixture* fixture)
	{
		b2Body* body = fixture->GetBody();
		if (body->GetType() == b2_dynamicBody)
		{
			bool inside = fixture->TestPoint(m_point);
			if (inside)
			{
				m_fixture = fixture;

				// We are done, terminate the query.
				return false;
			}
		}

		// Continue the query.
		return true;
	}

	b2Vec2 m_point;
	b2Fixture* m_fixture;
};

class Springs : public Test
{
public:
	Springs()
	{
		m_world->SetGravity(b2Vec2(0, 0));

		init();

		// create nodes
		b2BodyDef bd;
		bd.position.x = originX;
		bd.position.y = originY;
		m_node[0] = m_world->CreateBody(&bd);
		bd.position.x = originX + 2 * cosf(b2_pi / 2 - 2 * b2_pi / 6) * radius;
		bd.position.y = originY;
		m_node[1] = m_world->CreateBody(&bd);

		b2CircleShape shape;
		shape.m_radius = 1;
		m_node[0]->CreateFixture(&shape, 0.0f);
		m_node[1]->CreateFixture(&shape, 0.0f);

		// create droplets for all nodes
		for (int32 i = 0; i < 2; ++i)
		{
			for (int32 j = 0; j < 6; ++j)
			{
				int dropletID = m_nodeDroplets[i][j];

				if (NULL == m_droplet[dropletID])
				{
					// define shape (circle)
					b2CircleShape shape;
					shape.m_radius = 1;

					// define current element body
					b2BodyDef bd;
					bd.type = b2_dynamicBody;
          m_dropletDefPosition[dropletID] = b2Vec2(m_node[i]->GetPosition().x + radius * sinf(2 * b2_pi / 6 * j), m_node[i]->GetPosition().y + radius * cosf(2 * b2_pi / 6 * j));
					bd.position = m_dropletDefPosition[dropletID];

					// define fixture
					b2FixtureDef fd;
					fd.shape = &shape;

					// create body
					m_droplet[dropletID] = m_world->CreateBody(&bd);
					//m_shadeDroplet[dropletID] = m_world->CreateBody(&bd);

					// apply fixture
					m_droplet[dropletID]->CreateFixture(&fd);
					//m_shadeDroplet[dropletID]->CreateFixture(&fd);

					m_droplet[dropletID]->SetUserData((void*) dropletID);
					//m_shadeDroplet[dropletID]->SetUserData((void*) dropletID);

					//m_shadeDroplet[dropletID]->SetActive(false);
				}
			}
		}



		//for (int32 i = 0; i < elements; ++i)
		//{
		//	b2DistanceJointDef jointDef;
		//	jointDef.frequencyHz = 4.0f;
		//	jointDef.dampingRatio = 1;
		//	jointDef.collideConnected = true;
		//	
		//	jointDef.Initialize(m_bodies[m_first[i]], m_bodies[m_first[(i + 1) % elements]], m_bodies[m_first[i]]->GetPosition(), m_bodies[m_first[(i + 1) % elements]]->GetPosition());

		//	m_world->CreateJoint(&jointDef);

		//	jointDef.Initialize(m_bodies[m_first[i]], m_center[0], m_bodies[m_first[i]]->GetPosition(), m_center[0]->GetPosition());
		//	jointDef.frequencyHz = 3;
		//	jointDef.dampingRatio = 0.1f;
		//	m_world->CreateJoint(&jointDef)->SetUserData((void*) 100);
		//}

		//for (int32 i = 0; i < elements; ++i)
		//{
		//	b2DistanceJointDef jointDef;
		//	jointDef.frequencyHz = 4.0f;
		//	jointDef.dampingRatio = 1;
		//	jointDef.collideConnected = true;
		//	
		//	jointDef.Initialize(m_bodies[m_second[i]], m_bodies[m_second[(i + 1) % elements]], m_bodies[m_second[i]]->GetPosition(), m_bodies[m_second[(i + 1) % elements]]->GetPosition());

		//	m_world->CreateJoint(&jointDef);

		//	jointDef.Initialize(m_bodies[m_second[i]], m_center[1], m_bodies[m_second[i]]->GetPosition(), m_center[1]->GetPosition());
		//	jointDef.frequencyHz = 3;
		//	jointDef.dampingRatio = 0.1f;
		//	m_world->CreateJoint(&jointDef)->SetUserData((void*) 101);
		//}
	}

	void Step(Settings* settings)
	{
		Test::Step(settings);

		m_debugDraw.DrawString(5, m_textLine, "Selected: %d", m_selected);
		m_textLine += 15;

    for (int i = 0; i < 10; i++)
    {
		  if (m_defRotJoint[i])
		  {
			  m_defRotJoint[i]->SetTarget(m_dropletDefPosition[i]);
		  }
    }

		if (m_beginMoveJoint)
		{
			b2Vec2 d = m_beginMoveBody->GetPosition() - m_droplet[m_selected]->GetPosition();
			if (d.Length() >= 1.5f)
			{
				endPreMove();

				if ((m_droplet[m_selected]->GetPosition() - m_node[0]->GetPosition()).Length() < (m_droplet[m_selected]->GetPosition() - m_node[1]->GetPosition()).Length())
				{
					createJoints(0);
				}
				else
				{
					createJoints(1);
				}
			}

			m_debugDraw.DrawString(5, m_textLine, "Len: %f", d.Length());
			m_textLine += 15;
		}

		if (m_selected != -1)
		{
			//for (b2JointEdge* j = m_bodies[m_selected]->GetJointList(); j; j = j->next)
			//{
			//	if (j->joint->GetType() == e_distanceJoint && j->joint->GetUserData() != NULL)
			//	{
			//		b2DistanceJoint* joint = (b2DistanceJoint*) j->joint;

			//		b2Vec2 force = joint->GetReactionForce(1);
			//		m_debugDraw.DrawString(5, m_textLine, "Len: %f, F: %f %f (%f), Attached to: %d", joint->GetLength(), force.x, force.y, force.x * force.x + force.y * force.y, ((int) joint->GetUserData()) - 100);
			//		m_textLine += 15;
			//	}
			//}
		}
	}

	void MouseUp(const b2Vec2& p)
	{
		Test::MouseUp(p);

		if (m_selected != -1)
		{
      for (int i = 0; i < 10; ++i)
      {
			  b2MouseJointDef jointDef;
			  jointDef.dampingRatio = 0.95f;
			  jointDef.bodyA = m_groundBody;
			  jointDef.bodyB = m_droplet[i];
			  jointDef.target = m_droplet[i]->GetPosition();
			  jointDef.maxForce = 1000.0f * m_droplet[i]->GetMass();
			  m_defRotJoint[i] = (b2MouseJoint*) m_world->CreateJoint(&jointDef);
      }
    }

		endPreMove();

		m_selected = -1;
	}

	void MouseMove(const b2Vec2& p)
	{
	}

	void MouseDown(const b2Vec2& p)
	{
		Test::MouseDown(p);

		// Make a small box.
		b2AABB aabb;
		b2Vec2 d;
		d.Set(0.001f, 0.001f);
		aabb.lowerBound = p - d;
		aabb.upperBound = p + d;

		// Query the world for overlapping shapes.
		SpringQueryCallback callback(p);
		m_world->QueryAABB(&callback, aabb);

		if (callback.m_fixture)
		{
			b2Body* body = callback.m_fixture->GetBody();
			
			m_selected = (int) body->GetUserData();

			beginMove(m_selected);
			//for (int i = 0; i < 2; ++i)
			//{
			//	destroyJoints(i);
			//}

			//for (int i = 0; i < 2; ++i)
			//{
			//	if (m_dropletNodes[m_selected][i] != -1)
			//	{
			//		createJoints(m_dropletNodes[m_selected][i]);
			//	}
			//}
		}

    for (int i = 0; i < 10; i++)
    {
		  if (m_defRotJoint[i])
		  {
			  m_world->DestroyJoint(m_defRotJoint[i]);
			  m_defRotJoint[i] = NULL;
		  }
    }
  }

	void Keyboard(unsigned char key)
	{
		switch (key)
		{
		case 'c':

			//for (b2JointEdge* j = m_bodies[m_selected]->GetJointList(); j; j = j->next)
			//{
			//	if (j->joint->GetType() == e_distanceJoint && j->joint->GetUserData() != NULL)
			//	{
			//		b2DistanceJoint* joint = (b2DistanceJoint*) j->joint;

			//		//b2Vec2 force = joint->GetReactionForce(1);
			//		//m_debugDraw.DrawString(5, m_textLine, "Len: %f, F: %f %f (%f), Attached to: %d", joint->GetLength(), force.x, force.y, force.x * force.x + force.y * force.y, ((int) joint->GetUserData()) - 100);
			//		//m_textLine += 15;
			//	}
			//}
			break;
		}
	}

	static Test* Create()
	{
		return new Springs;
	}

	private:

		void init()
		{
      for (int i = 0; i < 10; i++)
      {
			  m_defRotJoint[i] = NULL;
      }
			m_beginMoveJoint = NULL;

			m_dropletNodes[0][0] = 0; m_dropletNodes[0][1] = -1;
			m_dropletNodes[1][0] = 0; m_dropletNodes[1][1] = 1;
			m_dropletNodes[2][0] = 0; m_dropletNodes[2][1] = 1;
			m_dropletNodes[3][0] = 0; m_dropletNodes[3][1] = -1;
			m_dropletNodes[4][0] = 0; m_dropletNodes[4][1] = -1;
			m_dropletNodes[5][0] = 0; m_dropletNodes[5][1] = -1;
			m_dropletNodes[6][0] = 1; m_dropletNodes[6][1] = -1;
			m_dropletNodes[7][0] = 1; m_dropletNodes[7][1] = -1;
			m_dropletNodes[8][0] = 0; m_dropletNodes[8][1] = 1;
			m_dropletNodes[9][0] = 0; m_dropletNodes[9][1] = 1;

			m_nodeDroplets[0][0] = 0;
			m_nodeDroplets[0][1] = 1;
			m_nodeDroplets[0][2] = 2;
			m_nodeDroplets[0][3] = 3;
			m_nodeDroplets[0][4] = 4;
			m_nodeDroplets[0][5] = 5;

			m_nodeDroplets[1][0] = 6;
			m_nodeDroplets[1][1] = 7;
			m_nodeDroplets[1][2] = 8;
			m_nodeDroplets[1][3] = 9;
			m_nodeDroplets[1][4] = 2;
			m_nodeDroplets[1][5] = 1;

			m_selected = -1;

			for (int i = 0; i < sizeof(m_droplet)/sizeof(m_droplet[0]); ++i)
			{
				m_droplet[i] = NULL;
				//m_shadeDroplet[i] = NULL;
			}
		}

		void createJoints(int nodeId)
		{
			for (int32 i = 0; i < 6; ++i)
			{
				b2DistanceJointDef jointDef;
				jointDef.frequencyHz = 4;
				jointDef.dampingRatio = 1;
				jointDef.collideConnected = true;
				
				jointDef.Initialize(m_droplet[m_nodeDroplets[nodeId][i]], m_droplet[m_nodeDroplets[nodeId][(i + 1) % 6]], m_droplet[m_nodeDroplets[nodeId][i]]->GetPosition(), m_droplet[m_nodeDroplets[nodeId][(i + 1) % 6]]->GetPosition());

				m_world->CreateJoint(&jointDef);

				jointDef.Initialize(m_droplet[m_nodeDroplets[nodeId][i]], m_node[nodeId], m_droplet[m_nodeDroplets[nodeId][i]]->GetPosition(), m_node[nodeId]->GetPosition());
				jointDef.frequencyHz = 4;
				jointDef.dampingRatio = 1;
				m_world->CreateJoint(&jointDef)->SetUserData((void*) 100);
			}
		}

		void destroyJoints(int nodeId)
		{
			for (int32 i = 0; i < 6; ++i)
			{
				b2Body* b = m_droplet[m_nodeDroplets[nodeId][i]];

				b2JointEdge* next = NULL;
				for (b2JointEdge* joint = b->GetJointList(); joint; )
				{
					next = joint->next;

					if (joint->joint->GetType() == e_distanceJoint)
					{
						m_world->DestroyJoint(joint->joint);
					}
					joint = next;
				}
			}
		}

		bool isDropletShared(int dropletId)
		{
			return m_dropletNodes[dropletId][1] != -1;
		}

		void beginMove(int dropletId)
		{
      destroyJoints(0);
      destroyJoints(1);

			if (isDropletShared(dropletId))
			{
				beginPreMove(dropletId);
			}
			else
			{
				createJoints(m_dropletNodes[dropletId][0]);
			}
		}

		void beginPreMove(int dropletId)
		{
			b2BodyDef bd;
			bd.position = m_droplet[dropletId]->GetPosition();
			m_beginMoveBody = m_world->CreateBody(&bd);
			b2CircleShape shape;
			shape.m_radius = 1;
			m_beginMoveBody->CreateFixture(&shape, 0.0f);

			b2DistanceJointDef jointDef;
			jointDef.frequencyHz = 4;
			jointDef.dampingRatio = 0.8f;
			jointDef.collideConnected = false;
			jointDef.bodyA = m_beginMoveBody;
			jointDef.bodyB = m_droplet[dropletId];
			jointDef.length = 0;
			jointDef.localAnchorA = m_beginMoveBody->GetPosition();
			jointDef.localAnchorB = m_droplet[dropletId]->GetPosition();

			m_beginMoveJoint = (b2DistanceJoint*) m_world->CreateJoint(&jointDef);
		}

		void endPreMove()
		{
			if (m_beginMoveJoint)
			{
				m_world->DestroyJoint(m_beginMoveJoint);
				m_beginMoveJoint = NULL;

				m_world->DestroyBody(m_beginMoveBody);
				m_beginMoveBody = NULL;
			}
		}

	private:

		b2Body* m_droplet[10];
		//b2Body* m_shadeDroplet[10];
		b2Body* m_node[2];

    b2Vec2 m_dropletDefPosition[10];

		int m_dropletNodes[10][2];
		int m_nodeDroplets[2][6];

		int m_selected;

		b2MouseJoint* m_defRotJoint[10];

		b2Body* m_beginMoveBody;
		b2DistanceJoint* m_beginMoveJoint;

		static const int originX = -8;
		static const int originY = 20;
		static const int radius  = 10;
};

#endif
