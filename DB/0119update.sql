USE [TQ3]
GO
/****** Object:  Table [dbo].[analys_group_user]    Script Date: 01/20/2016 17:08:57 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[analys_group_user](
	[id] [int] IDENTITY(1,1) NOT NULL,
	[groupid] [int] NULL,
	[clientid] [char](20) NULL,
	[analysid] [char](20) NULL
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[analys_group]    Script Date: 01/20/2016 17:08:57 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[analys_group](
	[groupid] [int] IDENTITY(1,1) NOT NULL,
	[analysid] [char](20) NULL,
	[groupname] [char](20) NULL
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  StoredProcedure [dbo].[analysgroup_mod_user]    Script Date: 01/20/2016 17:08:59 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create PROCEDURE [dbo].[analysgroup_mod_user]
	@groupid int,
	@clientid char(20),
	@analysid char(20)
AS
BEGIN
	SET NOCOUNT ON; 
	update analys_group_user set groupid=@groupid
	where clientid=@clientid and analysid=@analysid
END
GO
/****** Object:  StoredProcedure [dbo].[analysgroup_get_user]    Script Date: 01/20/2016 17:08:59 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create PROCEDURE [dbo].[analysgroup_get_user]
	@groupid int,
	@analysid char(20)
AS
BEGIN
	SET NOCOUNT ON; 
	select clientid from analys_group_user
	where groupid=@groupid and analysid=@analysid
END
GO
/****** Object:  StoredProcedure [dbo].[analysgroup_del_user]    Script Date: 01/20/2016 17:08:59 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create PROCEDURE [dbo].[analysgroup_del_user]
	@groupid int,
	@clientid char(20)
AS
BEGIN
	SET NOCOUNT ON; 
	delete from analys_group_user
	where groupid=@groupid and clientid=@clientid
END
GO
/****** Object:  StoredProcedure [dbo].[analysgroup_add_user]    Script Date: 01/20/2016 17:08:59 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
create PROCEDURE [dbo].[analysgroup_add_user]
	@groupid int,
	@clientid char(20),
	@analysid char(20)
AS
BEGIN
	SET NOCOUNT ON; 
	insert into analys_group_user(groupid,clientid,analysid)
	values(@groupid, @clientid, @analysid)
END
GO
/****** Object:  StoredProcedure [dbo].[analys_mod_group]    Script Date: 01/20/2016 17:08:59 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[analys_mod_group]
	@groupid int,
	@groupname char(20)
AS
BEGIN
	SET NOCOUNT ON; 
	update analys_group set groupname=@groupname where groupid=@groupid
END
GO
/****** Object:  StoredProcedure [dbo].[analys_get_group]    Script Date: 01/20/2016 17:08:59 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[analys_get_group]
	@analysid char(20)
AS
BEGIN
	SET NOCOUNT ON; 
	select groupid,groupname from analys_group where analysid=@analysid order by groupid
END
GO
/****** Object:  StoredProcedure [dbo].[analys_del_group]    Script Date: 01/20/2016 17:08:59 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[analys_del_group]
	@groupid int
AS
BEGIN
	SET NOCOUNT ON; 
	delete from analys_group where groupid=@groupid
END
GO
/****** Object:  StoredProcedure [dbo].[analys_add_group]    Script Date: 01/20/2016 17:08:59 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[analys_add_group]
	@analysid char(20),
	@groupname char(20)
AS
BEGIN
	SET NOCOUNT ON; 
	insert into analys_group(analysid,groupname)
	values(@analysid, @groupname)
END
GO
