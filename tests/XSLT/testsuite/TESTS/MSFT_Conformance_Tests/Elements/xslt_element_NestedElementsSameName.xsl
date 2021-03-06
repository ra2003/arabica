<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output omit-xml-declaration="yes" />

	<xsl:template match="/TESTCASES">
		<testmodule>
			<testcases>
				<xsl:apply-templates/>
			</testcases>
		</testmodule>
	</xsl:template>

	<xsl:template match="TESTCASES/TESTCASES">
		<testcase>
			<variations>			
				<xsl:apply-templates/>
			</variations>
		</testcase>
	</xsl:template>

	<xsl:template match="TEST">
		<variation>
			<xsl:attribute name="name">
				<xsl:value-of select="@ID"/>
			</xsl:attribute>
			<note/>
		</variation>
	</xsl:template>
</xsl:stylesheet>